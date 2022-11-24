module spi_contlr(button,clk,rst,RESET,/*SPI_DATARDY,*/SPI_BUSY,SPI_SCK,SPI_MOSI,SPI_SSEL,test_out);

input button;
input clk;
input rst;
//input SPI_DATARDY;
input SPI_BUSY;
output SPI_SCK;
output SPI_MOSI;
output SPI_SSEL;
output test_out;
output RESET;
reg [2:0] count_div_2_5M0Hz;
reg clk_2_5M0Hz;
wire stop;
wire ready;
wire [7:0] data;

always @(posedge clk or posedge rst)
begin
  if(rst) 
  begin
    count_div_2_5M0Hz <= 3'd0 ;
    clk_2_5M0Hz     <= 1'b0 ;
  end
  else if(count_div_2_5M0Hz == 3'd5) //for 25MHz divisor is 5 for 2.5mhz
  begin
    count_div_2_5M0Hz <= 3'd0 ;
    clk_2_5M0Hz     <= ~clk_2_5M0Hz; // Generating clock
  end  
  else 
  begin
    count_div_2_5M0Hz <= count_div_2_5M0Hz + 1;
    clk_2_5M0Hz    <= clk_2_5M0Hz;
  end   
end

//assign SPI_SCK=(clk_flag==0)?1:clk_2_5M0Hz;

spi_com sc(	.button(button)
				,.crystal_clk(clk)
				,.clk(clk_2_5M0Hz)
				,.real_spi_sck(SPI_SCK)
				,.rst(rst)							  
			//	,.SPI_DATARDY(SPI_DATARDY)
				,.SPI_BUSY(SPI_BUSY)
				,.SPI_MOSI(SPI_MOSI)
				,.SPI_SSEL(SPI_SSEL)
				,.RESET(RESET)
				,.stop(stop)
				,.din_byte(data)
				,.ready(ready)
				);	  	  
spi_test spit(.clk(clk_2_5M0Hz)
				,.rst(rst)							  
				,.stop(stop)
				,.data_out(data)
				,.data_in(8'h44)
				,.ready(ready)
				,.test_out(test_out)
				);	 		 
endmodule