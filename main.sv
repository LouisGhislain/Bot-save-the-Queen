module EncoderSpeed (
    input logic CLOCK_50,
    input logic reset,
    input logic encoder_a,
    input logic encoder_b,                                                    
    output logic [15:0] speed
);
    // Constants
    localparam int TIMER_MAX = 10000;

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

            // Calculate speed every TIMER_MAX cycles
            if (timer == TIMER_MAX) begin
                speed <= tick_count; // Assuming tick_count per TIMER_MAX = speed
                tick_count <= 0;
                timer <= 0;
            end
        end
    end
endmodule


module Odometer (                                                                  
    input logic CLOCK_50,
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
module SpiInterface (
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
    // Address parameters
    parameter ADDR_LEFT_SPEED = 8'h10;
    parameter ADDR_RIGHT_SPEED = 8'h11;
    parameter ADDR_LEFT_TICKS = 8'h12;
    parameter ADDR_RIGHT_TICKS = 8'h13;
    logic [7:0] address;
    logic [31:0] data_out;
    logic [31:0] data_in;

    logic [4:0] bit_count; // Count bits shifted
    logic transaction_active;
    logic [31:0] shift_register;

    always_ff @(posedge SCK or posedge reset) begin
    if (reset) begin
        address <= 0;
        bit_count <= 0;
        shift_register <= 32'b0;
        transaction_active <= 0;
    end else if (!CE1) begin
        transaction_active <= 1;
        bit_count <= bit_count + 1;

        // Shift in data
        shift_register <= {shift_register[30:0], MOSI};

        // Decode address when enough bits are received
        if (bit_count == 15) begin // Ensure full address has been received
            address <= shift_register[15:8];
            case (shift_register[15:8]) // Decode address immediately
                ADDR_LEFT_SPEED: shift_register <= {16'b0, left_speed};
                ADDR_RIGHT_SPEED: shift_register <= {16'b0, right_speed};
                ADDR_LEFT_TICKS: shift_register <= left_ticks;
                ADDR_RIGHT_TICKS: shift_register <= right_ticks;
                default: shift_register <= 32'b0; // Handle invalid addresses
            endcase
        end
    end else if (transaction_active) begin
        // Shift out data when active
        shift_register <= {shift_register[30:0], 1'b0};
        if (bit_count == 31) begin
            transaction_active <= 0; // End of transaction
            bit_count <= 0; // Reset bit counter
        end
    end
end

    assign MISO = shift_register[31]; // Send MSB

endmodule

/*
 * Module: top_module
 * Description: This module interfaces with GPIO pins for encoder and odometer inputs and SPI communication.
 *
 * Inputs:
 * - CLOCK_50: Clock signal (GPIO)
 * - reset: Reset signal (GPIO)
 * - ENC_1A: Encoder 1 Channel A (GPIO1)
 * - ENC_1B: Encoder 1 Channel B (GPIO3)
 * - ENC_2A: Encoder 2 Channel A (GPIO2)
 * - ENC_2B: Encoder 2 Channel B (GPIO5)
 * - ODO_1A: Odometer 1 Channel A (GPIO8)
 * - ODO_1B: Odometer 1 Channel B (GPIO9)
 * - ODO_2A: Odometer 2 Channel A (GPIO10)
 * - ODO_2B: Odometer 2 Channel B (GPIO11)
 * - SCK: SPI Clock (GPIO22)
 * - MOSI: SPI Master Out Slave In (GPIO23)
 * - CE1: SPI Chip Enable 1 (GPIO20)
 *
 * Outputs:
 * - MISO: SPI Master In Slave Out (GPIO21)
 */

module top_module (
    input logic CLOCK_50,
    input logic reset,
    input logic GPIO1,
    input logic GPIO3,
    input logic GPIO2,
    input logic GPIO5,
    input logic GPIO8,
    input logic GPIO9,
    input logic GPIO10,
    input logic GPIO11,
    input logic GPIO22,
    input logic GPIO23,
    input logic GPIO20,
    output logic GPIO21
);

    // Declare internal signals for GPIO assignments
    logic ENC_1A, ENC_1B, ENC_2A, ENC_2B;
    logic ODO_1A, ODO_1B, ODO_2A, ODO_2B;
    logic SCK, MOSI, CE1;

    // Assign GPIO pins to internal signals for better readability
    assign ENC_1A = GPIO1; // Encoder 1 Channel A
    assign ENC_1B = GPIO3; // Encoder 1 Channel B
    assign ENC_2A = GPIO2; // Encoder 2 Channel A
    assign ENC_2B = GPIO5; // Encoder 2 Channel B

    assign ODO_2A = GPIO10; // Odometer 2 Channel A
    assign ODO_2B = GPIO11; // Odometer 2 Channel B
    assign CE1 = GPIO20;    // SPI Chip Enable

    assign SCK = GPIO22;   // SPI Clock
    assign MOSI = GPIO23;  // Master Out Slave In
    assign MISO = GPIO21;  // Master In Slave Out

    // Internal signals
    logic [15:0] left_speed, right_speed;
    logic [31:0] left_ticks, right_ticks;

    // Instantiate modules
    EncoderSpeed left_encoder_speed (
        .CLOCK_50(CLOCK_50), .reset(reset), 
        .encoder_a(ENC_1A), .encoder_b(ENC_1B), 
        .speed(left_speed)
    );

    EncoderSpeed right_encoder_speed (
        .CLOCK_50(CLOCK_50), .reset(reset), 
        .encoder_a(ENC_2A), .encoder_b(ENC_2B), 
        .speed(right_speed)
    );

    Odometer left_odometer (
        .CLOCK_50(CLOCK_50), .reset(reset), 
        .encoder_a(ODO_1A), .encoder_b(ODO_1B), 
        .tick_count(left_ticks)
    );

    Odometer right_odometer (
        .CLOCK_50(CLOCK_50), .reset(reset), 
        .encoder_a(ODO_2A), .encoder_b(ODO_2B), 
        .tick_count(right_ticks)
    );

    SpiInterface spi (
        .CLOCK_50(CLOCK_50), .reset(reset),
        .SCK(SCK), .MOSI(MOSI), .CE1(CE1), .MISO(MISO),
        .left_speed(left_speed), .right_speed(right_speed),
        .left_ticks(left_ticks), .right_ticks(right_ticks)
    );

endmodule

