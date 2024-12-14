open System
open System.IO

type Property =
    { Ingredient: string
      Capacity: int
      Durability: int
      Flavor: int
      Texture: int
      Calories: int }

type Score =
    { Capacity: int
      Durability: int
      Flavor: int
      Texture: int }

    static member empty: Score =
        { Capacity = 0
          Durability = 0
          Flavor = 0
          Texture = 0 }

    static member total(s: Score) : int =
        if s.Capacity <= 0 || s.Durability <= 0 || s.Flavor <= 0 || s.Texture <= 0 then
            0
        else
            s.Capacity * s.Durability * s.Flavor * s.Texture

let parse (s: string) : Property =
    let parts =
        s
            .Replace(':', ' ')
            .Replace(',', ' ')
            .Split(' ', StringSplitOptions.RemoveEmptyEntries)

    { Ingredient = parts.[0]
      Capacity = int parts.[2]
      Durability = int parts.[4]
      Flavor = int parts.[6]
      Texture = int parts.[8]
      Calories = int parts.[10] }

let getScore (p: Property) (spoons: int) (acc: Score) : Score =
    { Capacity = p.Capacity * spoons + acc.Capacity
      Durability = p.Durability * spoons + acc.Durability
      Flavor = p.Flavor * spoons + acc.Flavor
      Texture = p.Texture * spoons + acc.Texture }

let rec findBestScore (properties: Property list) (remains: int) (spoons: int) (score: Score) : int =
    match properties with
    | [] -> failwith "never reach here"
    | h :: [] ->
        let score = getScore h spoons score
        Score.total score
    | h :: t ->
        let limit = spoons - remains + 1

        seq { 1..limit }
        |> Seq.fold
            (fun acc n ->
                let score = getScore h n score
                let ret = findBestScore t (remains - 1) (spoons - n) score
                max acc ret)
            0

let problem1 (input: Property list) : int =
    let remains = List.length input
    findBestScore input remains 100 Score.empty

let testInput =
    [ "Butterscotch: capacity -1, durability -2, flavor 6, texture 3, calories 8"
      "Cinnamon: capacity 2, durability 3, flavor -2, texture -1, calories 3" ]

let testData = testInput |> List.map parse
// 62842880
problem1 testData

let input = File.ReadAllLines("../input/day15.txt") |> Array.map parse |> Array.toList
let ret1 = problem1 input
printfn "ret1 = %d" ret1
