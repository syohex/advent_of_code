open System.IO

type Bag =
    { Red: int
      Green: int
      Blue: int }

    static member init() : Bag = { Red = 0; Green = 0; Blue = 0 }

    static member isPossible (a: Bag) (b: Bag) : bool =
        a.Red <= b.Red && a.Green <= b.Green && a.Blue <= b.Blue

let readInput (s: string) : string list = File.ReadLines(s) |> Seq.toList

let parseLine (s: string) : Bag list =
    s.Split(':').[1].Split(';')
    |> Array.map (fun s -> s.Trim())
    |> Array.map (fun s ->
        s.Split(',')
        |> Array.map (fun s -> s.Trim())
        |> Array.fold
            (fun acc s ->
                let d = s.Split(' ')

                match d.[1] with
                | "blue" -> { acc with Blue = int d.[0] }
                | "red" -> { acc with Red = int d.[0] }
                | "green" -> { acc with Green = int d.[0] }
                | _ -> failwith "never reach here")
            (Bag.init ()))
    |> Array.toList

let isGamePossible (bags: Bag list) : bool =
    let elfBag = { Red = 12; Green = 13; Blue = 14 }
    List.forall (fun bag -> Bag.isPossible bag elfBag) bags

let problem01 (input: string list) : int =
    input
    |> List.map parseLine
    |> List.indexed
    |> List.filter (fun (_, bags) -> isGamePossible bags)
    |> List.map (fun (i, _) -> i + 1)
    |> List.sum

let test1 () : int =
    let input =
        [ "Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green"
          "Game 2: 1 blue, 2 green; 3 green, 4 blue, 1 red; 1 green, 1 blue"
          "Game 3: 8 green, 6 blue, 20 red; 5 blue, 4 red, 13 green; 5 green, 1 red"
          "Game 4: 1 green, 3 red, 6 blue; 3 green, 6 red; 3 green, 15 blue, 14 red"
          "Game 5: 6 red, 1 blue, 3 green; 2 blue, 1 red, 2 green" ]

    problem01 input

// 8
test1 ()

let input = readInput "../input.txt"
problem01 input
