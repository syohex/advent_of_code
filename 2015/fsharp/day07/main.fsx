open System
open System.IO

type Operand =
    | Int of uint16
    | Symbol of string

type AssignOp =
    { Operand: Operand
      Destination: string }

type BitOp =
    { Op1: Operand
      Op2: Operand
      Destination: string }

type ShiftOp =
    { Op1: Operand
      Op2: Operand
      Destination: string }

type NotOp = { Op1: Operand; Destination: string }

type Op =
    | Assign of AssignOp
    | And of BitOp
    | Or of BitOp
    | LShift of ShiftOp
    | RShift of ShiftOp
    | Not of NotOp

let inline isNumber (s: string) = s |> Seq.forall Char.IsAsciiDigit

let inline toOperand (s: string) =
    if isNumber s then Int(uint16 s) else Symbol s

let parseInput (line: string) : Op =
    let parts = line.Split(' ')

    match parts with
    | [| op; "->"; dest |] ->
        Assign
            { Operand = toOperand op
              Destination = dest }
    | [| op1; "AND"; op2; "->"; dest |] ->
        And
            { Op1 = toOperand op1
              Op2 = toOperand op2
              Destination = dest }
    | [| op1; "OR"; op2; "->"; dest |] ->
        Or
            { Op1 = toOperand op1
              Op2 = toOperand op2
              Destination = dest }
    | [| op1; "LSHIFT"; op2; "->"; dest |] ->
        LShift
            { Op1 = toOperand op1
              Op2 = toOperand op2
              Destination = dest }
    | [| op1; "RSHIFT"; op2; "->"; dest |] ->
        RShift
            { Op1 = toOperand op1
              Op2 = toOperand op2
              Destination = dest }
    | [| "NOT"; op1; "->"; dest |] ->
        Not
            { Op1 = toOperand op1
              Destination = dest }
    | _ -> failwithf "unknown operation %A" parts

let inline findValue (op: Operand) (table: Map<string, uint16>) : Option<uint16> =
    match op with
    | Int v -> Some(v)
    | Symbol s -> Map.tryFind s table

let run (ops: Op list) (table: Map<string, uint16>) : Map<string, uint16> =
    let rec f ops table unresolved =
        match ops with
        | [] ->
            match unresolved with
            | [] -> table
            | _ -> f unresolved table []
        | h :: t ->
            match h with
            | Assign { Operand = op; Destination = d } ->
                match findValue op table with
                | Some(v) ->
                    match Map.tryFind d table with
                    | None -> f t (Map.add d v table) unresolved
                    | Some _ -> f t table unresolved
                | None -> f t table (h :: unresolved)
            | And { Op1 = op1
                    Op2 = op2
                    Destination = d } ->
                match findValue op1 table, findValue op2 table with
                | Some(v1), Some(v2) -> f t (Map.add d (v1 &&& v2) table) unresolved
                | _ -> f t table (h :: unresolved)
            | Or { Op1 = op1
                   Op2 = op2
                   Destination = d } ->
                match findValue op1 table, findValue op2 table with
                | Some(v1), Some(v2) -> f t (Map.add d (v1 ||| v2) table) unresolved
                | _ -> f t table (h :: unresolved)
            | LShift { Op1 = op1
                       Op2 = op2
                       Destination = d } ->
                match findValue op1 table, findValue op2 table with
                | Some(v1), Some(v2) -> f t (Map.add d (v1 <<< int v2) table) unresolved
                | _ -> f t table (h :: unresolved)
            | RShift { Op1 = op1
                       Op2 = op2
                       Destination = d } ->
                match findValue op1 table, findValue op2 table with
                | Some(v1), Some(v2) -> f t (Map.add d (v1 >>> int v2) table) unresolved
                | _ -> f t table (h :: unresolved)
            | Not { Op1 = op; Destination = d } ->
                match findValue op table with
                | Some(v) -> f t (Map.add d ~~~v table) unresolved
                | None -> f t table (h :: unresolved)

    f ops table []


let problem1 (input: Op list) : uint16 = run input Map.empty |> Map.find "a"

let problem2 (input: Op list) (initialB: uint16) : uint16 =
    let table = Map.empty |> Map.add "b" initialB
    run input table |> Map.find "a"

let testInput =
    [ "123 -> x"
      "456 -> y"
      "x AND y -> d"
      "x OR y -> e"
      "x LSHIFT 2 -> f"
      "y RSHIFT 2 -> g"
      "NOT x -> h"
      "NOT y -> i" ]
    |> List.map parseInput

run testInput

let input =
    File.ReadLines("../../input/day07.txt") |> Seq.map parseInput |> Seq.toList

let ret1 = problem1 input
// 16076
printfn "ret1=%d" ret1

let ret2 = problem2 input ret1
// 2797
printfn "ret2=%d" ret2
