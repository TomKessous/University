//
// Test Bench Module Tom_Dan_2_0_lib.register_file_tester.register_file_tester
//
// Created:
//          by - danbenam.UNKNOWN (L330W513)
//          at - 16:18:40 11/20/2021
//
// Generated by Mentor Graphics' HDL Designer(TM) 2019.2 (Build 5)
//
`resetall
`timescale 1ns/10ps
module register_file_tester (clk,
                             rst,
                             Rd_Wr_Id,
                             offset,
                             data_to_reg,
                             out,
                             registers
                            );

// Local declarations

parameter AMBA_WORD = 32;
parameter AMBA_ADDR_WIDTH = 20;
parameter DATA_WIDTH = 32;


output clk;
output rst;
output Rd_Wr_Id;
output offset;
output data_to_reg;
input  out;
input  registers;

logic                         clk;
logic                         rst;
logic [1:0]                   Rd_Wr_Id;
logic [AMBA_ADDR_WIDTH - 1:0] offset;
logic [AMBA_WORD - 1:0]       data_to_reg;
logic [AMBA_WORD - 1:0]       out;
wire  [AMBA_WORD - 1:0]       registers  [3:0];


endmodule // register_file_tester

