
`timescale 1ns/1ns
module spi_interface_tf_v_tf();

// DATE:     10:38:34 05/28/2009 
// MODULE:   spi_interface
// DESIGN:   spi_interface
// FILENAME: tf.v
// PROJECT:  spi_test_2
// VERSION:  

// Inputs
    reg clk;
    reg SPI_BUSY;
	 reg button;


// Outputs
    wire RESET;
    wire SPI_SCK;
    wire SPI_MOSI;
    wire SPI_SSEL;
    wire test_out;


// Bidirs


// Instantiate the UUT
    spi_interface uut (
        .clk(clk), 
		  .button(button),
        .RESET(RESET), 
        .SPI_BUSY(SPI_BUSY), 
        .SPI_SCK(SPI_SCK), 
        .SPI_MOSI(SPI_MOSI), 
        .SPI_SSEL(SPI_SSEL), 
        .test_out(test_out)
        );


// Initialize Inputs
   

        initial begin
		  button = 1;
            clk = 0;
            SPI_BUSY = 0;

				#3861000 button = 0;
				#1000 button = 1;
        end

always
#20 clk = ~clk;
   
endmodule

