module spi_slave(
	input	 logic			SPI_CLK,
								SPI_CS,
								SPI_MOSI,
	output logic			SPI_MISO,
	input logic [31:0]	ToSend,
	output logic [7:0]	Query,
	output logic			Ready,
	output logic [3:0]	DataAddr,
	output logic [31:0]	Data,
	output logic			Data_WE,
	input  logic			clk);
	
	
	// SPI SYNC
	
	logic SPI_CLK_sync;
	logic SPI_CS_sync;
	
	always_ff @(posedge clk) begin
		SPI_CLK_sync <= SPI_CLK;
		SPI_CS_sync  <= SPI_CS;
	end
	

	
	logic [39:0]		SPI_reg;
	assign Query 		= SPI_reg[7:0];
	assign Data 		= SPI_reg[31:0];
	assign DataAddr 	= SPI_reg[35:32];
		
	// SPI FSM
	
	typedef enum logic [1:0] {S0,S1,S2,S3} statetype;
	statetype state, nextstate;
	
	logic [5:0]	SPI_cnt;
	logic			SPI_cnt_reset, SPI_cnt_inc;
	logic			SPI_reg_reset, SPI_reg_shift, SPI_reg_load;
	logic 		MISO_we, MISO_reset;
	
	// State register & Bit counter & SPI register & MISO
	always_ff @(posedge clk) begin
		if (SPI_CS_sync)			state <= S0;
		else							state <= nextstate;
		
		if (SPI_cnt_reset)		SPI_cnt <= 6'd0;
		else if (SPI_cnt_inc)	SPI_cnt <= SPI_cnt + 6'd1;
		
		if (SPI_reg_reset)		SPI_reg <= 40'd0;
		else if (SPI_reg_shift) SPI_reg <= {SPI_reg[38:0], SPI_MOSI};
		else if (SPI_reg_load)  SPI_reg <= {ToSend, SPI_reg[7:0]};
		
		if (MISO_reset)			SPI_MISO <= 0;
		else if (SPI_reg_load)	SPI_MISO <= ToSend[31];
		else if (MISO_we)		SPI_MISO <= SPI_reg[39];
	end
	
	// Next state logic
	always_comb begin
		
		//Default value
		nextstate = state;
		SPI_cnt_reset = 0; SPI_cnt_inc = 0;
		SPI_reg_reset = 0; SPI_reg_shift = 0; SPI_reg_load = 0;
		MISO_we = 0; MISO_reset = 0;
		Data_WE = 0;
		Ready = 0;
		
		case (state)
			S0: if (~SPI_CS_sync) begin
					SPI_cnt_reset = 1;
					SPI_reg_reset = 1;
					MISO_reset = 1;
					nextstate = S1;
				end
			S1: if (SPI_CLK_sync) begin
					SPI_reg_shift = 1;
					SPI_cnt_inc = 1;
					nextstate = S2;
				end
			S2: if (~SPI_CLK_sync) begin
					MISO_we = 1;
					if (SPI_cnt == 8) begin
						SPI_reg_load = 1;
						Ready = 1;
					end
					if (SPI_cnt == 40) begin
						if (SPI_reg[39] == 1) Data_WE = 1;
						nextstate = S3;
					end
					else nextstate = S1;
				end
			S3: if (SPI_CS_sync) nextstate = S0;
		endcase
	end
					
endmodule
