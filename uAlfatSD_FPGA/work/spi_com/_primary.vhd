library verilog;
use verilog.vl_types.all;
entity spi_com is
    generic(
        init            : integer := 0;
        \RST\           : integer := 1;
        wait_0          : integer := 2;
        wait_1          : integer := 3;
        idle            : integer := 4;
        spi_delay       : integer := 5;
        write           : integer := 6;
        check_command   : integer := 7
    );
    port(
        button          : in     vl_logic;
        crystal_clk     : in     vl_logic;
        clk             : in     vl_logic;
        real_spi_sck    : out    vl_logic;
        rst             : in     vl_logic;
        reset           : out    vl_logic;
        spi_busy        : in     vl_logic;
        spi_mosi        : out    vl_logic;
        spi_ssel        : out    vl_logic;
        stop            : in     vl_logic;
        din_byte        : in     vl_logic_vector(7 downto 0);
        ready           : out    vl_logic
    );
end spi_com;
