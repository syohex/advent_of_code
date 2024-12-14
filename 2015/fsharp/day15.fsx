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
      Texture: int
      Calorie: int }

    static member empty: Score =
        { Capacity = 0
          Durability = 0
          Flavor = 0
          Texture = 0
          Calorie = 0 }

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
      Texture = p.Texture * spoons + acc.Texture
      Calorie = p.Calories * spoons + acc.Calorie }

let rec findBestScore (properties: Property list) (spoons: int) (predicate: Score -> bool) (score: Score) : int =
    match properties with
    | [] -> failwith "never reach here"
    | h :: [] ->
        let score = getScore h spoons score
        if predicate score then Score.total score else 0
    | h :: t ->
        let limit = spoons - (List.length t)

        seq { 1..limit }
        |> Seq.fold
            (fun acc n ->
                let score = getScore h n score
                let ret = findBestScore t (spoons - n) predicate score
                max acc ret)
            0

let problem1 (input: Property list) : int =
    findBestScore input 100 (fun _ -> true) Score.empty

let problem2 (input: Property list) : int =
    findBestScore input 100 (fun s -> s.Calorie = 500) Score.empty

let testInput =
    [ "Butterscotch: capacity -1, durability -2, flavor 6, texture 3, calories 8"
      "Cinnamon: capacity 2, durability 3, flavor -2, texture -1, calories 3" ]

let testData = testInput |> List.map parse
// 62842880
problem1 testData
// 57600000
problem2 testData

let input =
    File.ReadAllLines("../input/day15.txt") |> Array.map parse |> Array.toList

let ret1 = problem1 input
// 21367368
printfn "ret1 = %d" ret1

let ret2 = problem2 input
// 1766400
printfn "ret2 = %d" ret2
