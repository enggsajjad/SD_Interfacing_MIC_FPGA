`define d_sample 24'h7FFFFF
module spi_test(clk
					,rst							  
					,stop
					,data_out
					,data_in
					,ready
					,test_out
					);

input clk;
input rst;
input ready;
input [7:0] data_in;
output stop;
output [7:0] data_out;
output test_out;
reg [7:0] data_out;
reg [4:0] ch_count;
reg [23:0] data_count;





always @(negedge clk or posedge rst)
begin
   if(rst)
	 begin
      data_out <= 8'd0;
		//flag_nibl<=1'b0;
	 end

   else if(ready)
	 begin
	   case(ch_count)
		 				
						5'd0:  data_out<=8'd73; // I
						5'd1:  data_out<=8'h0D; // enter

						5'd2:  data_out<=8'd79; // O
					  	5'd3:  data_out<=8'd32; // space
					  	5'd4:  data_out<=8'd49; // 1
					  	5'd5:  data_out<=8'd87; // W
					  	5'd6:  data_out<=8'd62; // >
					  	5'd7:  data_out<=8'd68; // D
					  	5'd8:  data_out<=8'd65; // A
					  	5'd9:  data_out<=8'd46; // .
					  	5'd10: data_out<=8'd76; // L
					  	5'd11: data_out<=8'd79; // O
					  	5'd12: data_out<=8'd71; // G
					  	5'd13: data_out<=8'h0D;// enter
						
						5'd14: data_out<=8'd87; // W
						5'd15: data_out<=8'd32; // space
						5'd16: data_out<=8'd49; // 1
						5'd17: data_out<=8'd62; // >
					  
					   //5'd18: data_out<=8'd48; // 5
					//	5'd18: data_out<=8'd49; // 1
					//	5'd20: data_out<=8'd48; // 0 
					//	5'd19: data_out<=8'h0D; // enter 
					//	5'd20: data_out<=data_in;													

					//	5'd21: data_out<=8'd67; // C
					  //	5'd22: data_out<=8'd32;  //space
					//	5'd23: data_out<=8'd49; // 1	 
					 // 	5'd24: data_out<=8'h0D; // enter
					
						5'd18: data_out<=8'd56; // 8
						5'd19: data_out<=8'd48; // 0
						5'd20: data_out<=8'd48; // 0 
						5'd21: data_out<=8'd48; // 0
						5'd22: data_out<=8'd48; // 0
						5'd23: data_out<=8'd48; // 0
						//5'd22: data_out<=8'd48; // 0 
						//5'd23: data_out<=8'd48; // 0  
						5'd24: data_out<=8'h0D; // enter 
						5'd25: data_out<=data_in;													

						5'd26: data_out<=8'd67; // C
					  	5'd27: data_out<=8'd32;  //space
						5'd28: data_out<=8'd49; // 1	 
					  	5'd29: data_out<=8'h0D;   // enter	 				
					  					
		  				default : data_out <=   8'd0;
										
		endcase       
	 end					 
   else
		begin
      data_out <= data_out;
		end
end

always @(negedge clk or posedge rst)
begin
   if(rst)
      ch_count <= 5'd0;
   else if ((ready==1 && ch_count<=5'd24/*21*/)||(ready==1 && ch_count<=5'd29/*26*/ && data_count==`d_sample))
		ch_count<=ch_count+1;
	else
		ch_count<=ch_count;	 
end

always @(negedge clk or posedge rst)
begin
if(rst) 
	data_count<=0;
else if(ready==1 && ch_count==5'd25/*22*/ && data_count<`d_sample) 
	data_count<=data_count+1;
else
	data_count<=data_count;
end
assign test_out=(ch_count==5'd30/*27*/)?1:0;
assign stop=(ch_count==5'd30/*27*/)?1:0;        
endmodule