module spi_com(button,crystal_clk,clk,real_spi_sck,rst,RESET/*,SPI_DATARDY*/,SPI_BUSY,SPI_MOSI,SPI_SSEL,stop,din_byte,ready);

input button;
input crystal_clk;
input clk;
output real_spi_sck;
input rst;
//input SPI_DATARDY;
input SPI_BUSY;
output SPI_MOSI;
output SPI_SSEL;
output RESET;
input stop;
output ready;



reg clk_flag;

parameter INIT=3'd0;
parameter RST=3'd1;
parameter WAIT_0=3'd2;
parameter WAIT_1=3'd3;
parameter IDLE=3'd4;
parameter SPI_DELAY=3'd5;
parameter WRITE=3'd6;
parameter Check_Command=3'd7;


reg [23:0] c_timer,n_timer;
reg [2:0] c_state,n_state;
reg [7:0] data_buf;
input [7:0] din_byte;
reg flag;
reg [3:0] shift_count;
wire rst_n;
reg [8:0] smooth_cntr;
reg[1:0]  smooth_reg;
wire clk_flag_sync;
wire spi_sck;

wire 	real_spi_sck;
wire command;
reg [2:0] button_reg = 3'b111;


always@(negedge clk or posedge rst_n)
	if(rst_n)
		button_reg <= 3'b111;
	else 
	 	button_reg <= {button_reg[1:0] , button};

assign command = ~(button_reg[2]) & button_reg[1];


always@(posedge crystal_clk or posedge rst_n)
	if(rst_n)
		smooth_reg <= 0;
	else if(clk_flag)
	 	smooth_reg <= {smooth_reg[0] ,clk_flag};

assign clk_flag_sync = ~(smooth_reg[1]) & smooth_reg[0];
always@(posedge crystal_clk or posedge rst_n)
	if(rst_n)
		smooth_cntr <= 96;
	else if(clk_flag_sync)
	 	smooth_cntr <= 0;
	else if(smooth_cntr == 96 )
		smooth_cntr<= smooth_cntr;
	else
		smooth_cntr <= smooth_cntr+1;

// Counter that count shift in Data Buffer Logic
always @(negedge spi_sck or posedge rst_n)
begin
   if(rst_n)
      shift_count <= 4'd0;   
   else 
      shift_count <= shift_count + 1;   		 

end

// strat bit pipelinin
always @(negedge spi_sck or posedge rst)
begin
   if(rst)
      data_buf <= 8'd0;
	else if(shift_count==0)
    	data_buf <= din_byte;           //load din_byte at 1st -ve edge  
   else 
      data_buf <= {data_buf[6:0],1'b0};
end  


always @(negedge clk or posedge rst)
begin
	if(rst) c_state<=INIT;
	else 
	begin
	c_state<=n_state;
	c_timer<=n_timer;
	end
end

always @(c_state or c_timer  or SPI_BUSY or stop/*or SPI_DATARDY */or shift_count or command)
begin

	case(c_state)
	INIT:	begin
				n_timer<=24'd0;
				n_state<=RST;
				clk_flag<=1'b0;
			end
	RST:	begin
				if(c_timer==24'd15) 
					begin
						n_timer<=24'd0;
						n_state<=WAIT_0;
					end
					else
					begin
					n_timer<=c_timer+1;
					clk_flag<=1'b0;
					end
			end
	WAIT_0:	begin
					if(c_timer==24'd8000) 
					begin
						n_timer<=24'd0;
						n_state<=Check_Command;
					end
					else
					begin
					n_timer<=c_timer+1;
					clk_flag<=1'b0;
					end
				end	

	Check_Command:
		begin
			 clk_flag<=1'b0;
			 flag<=1'b0;
			 if(command ==1)
				n_state<=WAIT_1;
		end			 
	
	WAIT_1:	begin
				clk_flag<=0;
				flag<=1'b1;
				if(c_timer==24'd5)  // 2us
			 		begin
			 		n_timer<=24'd0;
			 		n_state<=IDLE;
			 		end
				else n_timer<=c_timer+1; 
				end	  	
	IDLE:	begin
				clk_flag<=1'b0;
				flag<=1'b0;
				if(SPI_BUSY==0 && stop==0) 
					n_state<=SPI_DELAY;		
			end
	SPI_DELAY:	begin						  //0.4=0.5us
						clk_flag<=1'b0;
						flag<=1'b0;
						n_state<=WRITE;
					end
	WRITE:	begin
						if(shift_count<=4'd8) clk_flag<=1;
						else  
							begin 
								n_state<=WAIT_1;	
								clk_flag<=0;  
							end 
				end

	default:	n_state <= INIT;	 
	endcase
	
end

assign real_spi_sck = (clk_flag == 1 && smooth_cntr < 93 )? clk : 1;
assign spi_sck=(clk_flag==0 )?1:clk;
assign rst_n=(rst==1 || flag==1)?1:0;
assign SPI_MOSI=data_buf[7];
assign ready=(c_state==IDLE && SPI_BUSY==0)?1:0;
assign SPI_SSEL=(c_state==SPI_DELAY || c_state==WRITE)?0:1;//(c_state==INIT || c_state==RST || c_state==WAIT_0)?1:0;
assign RESET=(c_state==RST)?0:1;
endmodule