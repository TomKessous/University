//
// Test Bench Module Tom_Dan_2_0_lib.encode_H1_tester.encode_H1_tester
//
// Created:
//          by - danbenam.UNKNOWN (L330W513)
//          at - 10:56:51 11/26/2021
//
// Generated by Mentor Graphics' HDL Designer(TM) 2019.2 (Build 5)
//
`resetall
`timescale 1ns/10ps
module encode_H1_tester (DATA_IN,
                         CodeWord
                        );

// Local declarations

parameter DATA_WIDTH = 32;


output DATA_IN;
input  CodeWord;

logic [3:0]              DATA_IN;
logic [DATA_WIDTH - 1:0] CodeWord;


endmodule // encode_H1_tester


