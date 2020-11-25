package main

import (
  "./parser"
  "github.com/antlr/antlr4/runtime/Go/antlr"
  "fmt"
  "strconv"
)

type calcListener struct {
  *parser.BaseCalcListener

  stack []int
}

func (l *calcListener) push(i int) {
  l.stack = append(l.stack, i)
}

func (l *calcListener) pop() int {
  if len(l.stack) < 1 {
    panic("stack is empty unable to pop")
  }

  // Get the last value from the stack.
  result := l.stack[len(l.stack)-1]

  // Remove the last element from the stack.
  l.stack = l.stack[:len(l.stack)-1]

  return result
}

func (l *calcListener) EnterNumber(ctx *parser.NumberContext) {
  number, _ := strconv.Atoi(ctx.GetText())
  l.push(number)
  fmt.Printf("PUSH %s\n", ctx.GetText())
}

func (l *calcListener) ExitBinaryOperation(ctx *parser.BinaryOperationContext) {
  arg2 := l.pop()
  arg1 := l.pop()
  result := 0

  switch ctx.GetOp().GetTokenType() {
  case parser.CalcParserMUL:
    fmt.Printf("MUL(%d, %d)\n", arg1, arg2)
    result = arg1 * arg2
  case parser.CalcParserADD:
    fmt.Printf("ADD(%d, %d)\n", arg1, arg2)
    result = arg1 + arg2
  }

  l.push(result)
}

func calc(input string) int {
  // Setup the input
  is := antlr.NewInputStream(input)

  // Create the Lexer
  lexer := parser.NewCalcLexer(is)
  stream := antlr.NewCommonTokenStream(lexer, antlr.TokenDefaultChannel)

  // Create the Parser
  p := parser.NewCalcParser(stream)

  // Finally parse the expression (by walking the tree)
  var listener calcListener
  antlr.ParseTreeWalkerDefault.Walk(&listener, p.Start())

  return listener.pop()
}

func main() {
  fmt.Printf("%d\n", calc("1 + 2 * 3 + 4"))
}