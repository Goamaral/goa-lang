// Code generated from Calc.g4 by ANTLR 4.8. DO NOT EDIT.

package parser // Calc

import "github.com/antlr/antlr4/runtime/Go/antlr"

// BaseCalcListener is a complete listener for a parse tree produced by CalcParser.
type BaseCalcListener struct{}

var _ CalcListener = &BaseCalcListener{}

// VisitTerminal is called when a terminal node is visited.
func (s *BaseCalcListener) VisitTerminal(node antlr.TerminalNode) {}

// VisitErrorNode is called when an error node is visited.
func (s *BaseCalcListener) VisitErrorNode(node antlr.ErrorNode) {}

// EnterEveryRule is called when any rule is entered.
func (s *BaseCalcListener) EnterEveryRule(ctx antlr.ParserRuleContext) {}

// ExitEveryRule is called when any rule is exited.
func (s *BaseCalcListener) ExitEveryRule(ctx antlr.ParserRuleContext) {}

// EnterStart is called when production start is entered.
func (s *BaseCalcListener) EnterStart(ctx *StartContext) {}

// ExitStart is called when production start is exited.
func (s *BaseCalcListener) ExitStart(ctx *StartContext) {}

// EnterNumber is called when production Number is entered.
func (s *BaseCalcListener) EnterNumber(ctx *NumberContext) {}

// ExitNumber is called when production Number is exited.
func (s *BaseCalcListener) ExitNumber(ctx *NumberContext) {}

// EnterBinaryOperation is called when production BinaryOperation is entered.
func (s *BaseCalcListener) EnterBinaryOperation(ctx *BinaryOperationContext) {}

// ExitBinaryOperation is called when production BinaryOperation is exited.
func (s *BaseCalcListener) ExitBinaryOperation(ctx *BinaryOperationContext) {}
