library verilog;
use verilog.vl_types.all;
entity spi_test is
    port(
        clk             : in     vl_logic;
        rst             : in     vl_logic;
        stop            : out    vl_logic;
        data_out        : out    vl_logic_vector(7 downto 0);
        data_in         : in     vl_logic_vector(7 downto 0);
        ready           : in     vl_logic;
        test_out        : out    vl_logic
    );
end spi_test;
