module spi_interface(button,clk,RESET/*,SPI_DATARDY*/,SPI_BUSY,SPI_SCK,SPI_MOSI,SPI_SSEL,test_out);
input button;
input clk;
//input SPI_DATARDY;
input SPI_BUSY;
output SPI_SCK;
output SPI_MOSI;
output SPI_SSEL;
output RESET;
output test_out;
wire rst_i;
wire int_clkin;
wire lock;
wire int_clk1x;
wire int_clk1x_b;
wire clk_i;
reg [10:0] count =0;

always@(posedge clk )
	if(count == 1000)
		count <= count;
	else
		count <= count + 1;
						  
assign rst_i = ((count >= 200) && (count <= 300) ) ? 1 : 0;
	 											  
/*always@(posedge int_clkin)
	 begin
	  if(lock==0) rst_i<=1;	// keep in reset untill dll start up and lock
	  else rst_i<=0;   // release reset once dlls lock
	 end
  */
// master clock must come from a dedicated clock pin
// IBUFG clkin_buf(.I(clk),.O(int_clkin));

// sync'ed single, doubled and divided clocks for use by internal logic
// BUFG  int_clk1x_buf(.I(int_clk1x),.O(int_clk1x_b));
//if IN_PHASE  generate an internal clock sync'ed to the master clock
/*   CLKDLL dllint 
      (
        .CLKIN(int_clkin),
        .CLKFB(int_clk1x_b),
        .CLK0(int_clk1x),
        .RST(1'b0),
        .CLK90(),
        .CLK180(),
        .CLK270(),
        .CLK2X(),
        .CLKDV(),	 // by default divide/2
        .LOCKED(lock)
        );	 	 				  */
		  

spi_contlr spi(.clk(clk/*clk_i*/),
					.button(button),
						.rst(rst_i),
						//.SPI_DATARDY(SPI_DATARDY),
						.SPI_BUSY(SPI_BUSY),
						.SPI_SCK(SPI_SCK),
						.SPI_MOSI(SPI_MOSI),
						.SPI_SSEL(SPI_SSEL),
						.RESET(RESET),
						.test_out(test_out)
						);		 

	
//assign clk_i  = int_clk1x_b;
endmodule