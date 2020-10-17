// Code generated from Calc.g4 by ANTLR 4.8. DO NOT EDIT.

package parser // Calc

import "github.com/antlr/antlr4/runtime/Go/antlr"

// CalcListener is a complete listener for a parse tree produced by CalcParser.
type CalcListener interface {
	antlr.ParseTreeListener

	// EnterStart is called when entering the start production.
	EnterStart(c *StartContext)

	// EnterNumber is called when entering the Number production.
	EnterNumber(c *NumberContext)

	// EnterBinaryOperation is called when entering the BinaryOperation production.
	EnterBinaryOperation(c *BinaryOperationContext)

	// ExitStart is called when exiting the start production.
	ExitStart(c *StartContext)

	// ExitNumber is called when exiting the Number production.
	ExitNumber(c *NumberContext)

	// ExitBinaryOperation is called when exiting the BinaryOperation production.
	ExitBinaryOperation(c *BinaryOperationContext)
}
