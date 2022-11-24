library verilog;
use verilog.vl_types.all;
entity spi_contlr is
    port(
        button          : in     vl_logic;
        clk             : in     vl_logic;
        rst             : in     vl_logic;
        reset           : out    vl_logic;
        spi_busy        : in     vl_logic;
        spi_sck         : out    vl_logic;
        spi_mosi        : out    vl_logic;
        spi_ssel        : out    vl_logic;
        test_out        : out    vl_logic
    );
end spi_contlr;
