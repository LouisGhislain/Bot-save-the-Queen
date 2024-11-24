module encoder_speed (
    input logic CLOCK_50,
    input logic reset,
    input logic encoder_a,
    input logic encoder_b,                                                    
    output logic [15:0] speed
);
    // Internal variables for tick counting and time interval
    logic [15:0] tick_count;
    logic [15:0] timer;

    always_ff @(posedge CLOCK_50 or posedge reset) begin
        if (reset) begin
            tick_count <= 0;
            timer <= 0;
            speed <= 0;
        end else begin
            // Increment timer and count encoder ticks
            timer <= timer + 1;

            if (encoder_a ^ encoder_b) // Detect tick   
                tick_count <= tick_count + 1;

            // Calculate speed every 10ms
            if (timer == 10000) begin
                speed <= tick_count; // Assuming tick_count per 10ms = speed
                tick_count <= 0;
                timer <= 0;
            end
        end
    end
endmodule


module odometer (                                                                  
    input logic CLOCK_50,                                                               //TO DO: test the outputs in the .py on the rasp
    input logic reset,
    input logic encoder_a,
    input logic encoder_b,
    output logic [31:0] tick_count
);
    always_ff @(posedge CLOCK_50 or posedge reset) begin
        if (reset) begin
            tick_count <= 0;
        end else if (encoder_a ^ encoder_b) begin
            // Increment tick count on encoder signal change
            tick_count <= tick_count + 1;
        end
    end
endmodule


module spi_interface (
    input logic CLOCK_50,
    input logic reset,
    input logic SCK,
    input logic MOSI,
    input logic CE1,
    output logic MISO,
    input logic [15:0] left_speed,
    input logic [15:0] right_speed,
    input logic [31:0] left_ticks,
    input logic [31:0] right_ticks
);
    logic [7:0] address;
    logic [31:0] data_out;
    logic [31:0] data_in;

    always_ff @(posedge SCK or posedge reset) begin
        if (reset) begin
            address <= 0;
            data_out <= 0;
        end else if (!CE1) begin
            // Handle SPI communication
            // Address decoding
            case (address)
                8'h10: data_out <= {16'b0, left_speed};
                8'h11: data_out <= {16'b0, right_speed};
                8'h12: data_out <= left_ticks;
                8'h13: data_out <= right_ticks;
            endcase
        end
    end
    assign MISO = data_out[31]; // Simplified MISO logic
endmodule


module top_module (
    input logic CLOCK_50,
    input logic reset,
    input logic ENC_1A,
    input logic ENC_1B,
    input logic ENC_2A,
    input logic ENC_2B,
    input logic ODO_1A,
    input logic ODO_1B,
    input logic ODO_2A,
    input logic ODO_2B,
    input logic SCK,                // SPI clock
    input logic MOSI,
    input logic CE1,
    output logic MISO
);
    // Internal signals
    logic [15:0] left_speed, right_speed;
    logic [31:0] left_ticks, right_ticks;

    // Instantiate modules
    encoder_speed left_encoder_speed (
        .CLOCK_50(CLOCK_50), .reset(reset), 
        .encoder_a(ENC_1A), .encoder_b(ENC_1B), 
        .speed(left_speed)
    );

    encoder_speed right_encoder_speed (
        .CLOCK_50(CLOCK_50), .reset(reset), 
        .encoder_a(ENC_2A), .encoder_b(ENC_2B), 
        .speed(right_speed)
    );

    odometer left_odometer (
        .CLOCK_50(CLOCK_50), .reset(reset), 
        .encoder_a(ODO_1A), .encoder_b(ODO_1B), 
        .tick_count(left_ticks)
    );

    odometer right_odometer (
        .CLOCK_50(CLOCK_50), .reset(reset), 
        .encoder_a(ODO_2A), .encoder_b(ODO_2B), 
        .tick_count(right_ticks)
    );

    spi_interface spi (
        .CLOCK_50(CLOCK_50), .reset(reset),
        .SCK(SCK), .MOSI(MOSI), .CE1(CE1), .MISO(MISO),
        .left_speed(left_speed), .right_speed(right_speed),
        .left_ticks(left_ticks), .right_ticks(right_ticks)
    );
endmodule
