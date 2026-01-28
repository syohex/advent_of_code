open System
open System.IO
open System.Text.RegularExpressions

type Point =
    { position: int * int
      velocity: int * int }

// An input line is like "position=< 9,  1> velocity=< 0,  2>"
let parseInput (s: string) : Point =
    let pattern =
        @"^position=<\s*(?<positionX>-?\d+),\s*(?<positionY>-?\d+)>\s+velocity=<\s*(?<velocityX>-?\d+),\s*(?<velocityY>-?\d+)>"

    let m = Regex.Match(s, pattern)

    if m.Success then
        { position = int m.Groups.["positionX"].Value, int m.Groups.["positionY"].Value
          velocity = int m.Groups.["velocityX"].Value, int m.Groups.["velocityY"].Value }
    else
        failwithf "cannot parse input line => '%s'" s

// parseInput "position=< -9,  -1> velocity=< -0,  -2>"

let normalize (points: Point list) : Point list * int * int =
    let minX, minY, maxX, maxY =
        points
        |> List.fold
            (fun (minX, minY, maxX, maxY) { position = x, y; velocity = _ } ->
                min minX x, min minY y, max maxX x, max maxY y)
            (Int32.MaxValue, Int32.MaxValue, Int32.MinValue, Int32.MinValue)

    let points' =
        points
        |> List.map (fun { position = x, y; velocity = v } ->
            { position = x - minX, y - minY
              velocity = v })

    points', maxX - minX, maxY - minY

let draw (points: Point list) : unit =
    let points, xSize, ySize = normalize points

    let pointSet =
        points
        |> List.fold (fun acc { position = x, y; velocity = _ } -> Set.add (x, y) acc) Set.empty

    for y in 0..ySize do
        for x in 0..xSize do
            if Set.contains (x, y) pointSet then
                printf "#"
            else
                printf "."

        printfn ""

    ()

let update (points: Point list) : Point list =
    points
    |> List.map (fun { position = row, col; velocity = x, y } ->
        { position = row + x, col + y
          velocity = x, y })

let adjacentPointDistance (x: int) (y: int) (points: Point list) : int =
    let rec f points minDist =
        match points with
        | [] -> minDist
        | { position = x1, y1; velocity = _ } :: t ->
            if x = x1 && y = y1 then
                f t minDist
            else
                let dist = abs (x - x1) + abs (y - y1)
                f t (min minDist dist)

    f points Int32.MaxValue

let isAppearedInTheSky (points: Point list) : bool =
    let rec collectDistances ps acc =
        match ps with
        | []
        | _ :: [] -> Some acc
        | { position = x, y; velocity = _ } :: t ->
            let minDist = adjacentPointDistance x y points

            if minDist >= 3 then
                None
            else
                let v = Map.tryFind minDist acc |> Option.defaultValue 0
                collectDistances t (Map.add minDist (v + 1) acc)

    match collectDistances points Map.empty with
    | None -> false
    | Some distances -> Map.count distances <= 2

let problem1 (points: Point list) : unit =
    let rec f points =
        if isAppearedInTheSky points then
            draw points
        else
            f (update points)

    f points

let problem2 (points: Point list) : int =
    let rec f i points =
        if isAppearedInTheSky points then
            draw points
            i
        else
            f (i + 1) (update points)

    f 0 points

let testData =
    [ "position=< 9,  1> velocity=< 0,  2>"
      "position=< 7,  0> velocity=<-1,  0>"
      "position=< 3, -2> velocity=<-1,  1>"
      "position=< 6, 10> velocity=<-2, -1>"
      "position=< 2, -4> velocity=< 2,  2>"
      "position=<-6, 10> velocity=< 2, -2>"
      "position=< 1,  8> velocity=< 1, -1>"
      "position=< 1,  7> velocity=< 1,  0>"
      "position=<-3, 11> velocity=< 1, -2>"
      "position=< 7,  6> velocity=<-1, -1>"
      "position=<-2,  3> velocity=< 1,  0>"
      "position=<-4,  3> velocity=< 2,  0>"
      "position=<10, -3> velocity=<-1,  1>"
      "position=< 5, 11> velocity=< 1, -2>"
      "position=< 4,  7> velocity=< 0, -1>"
      "position=< 8, -2> velocity=< 0,  1>"
      "position=<15,  0> velocity=<-2,  0>"
      "position=< 1,  6> velocity=< 1,  0>"
      "position=< 8,  9> velocity=< 0, -1>"
      "position=< 3,  3> velocity=<-1,  1>"
      "position=< 0,  5> velocity=< 0, -1>"
      "position=<-2,  2> velocity=< 2,  0>"
      "position=< 5, -2> velocity=< 1,  2>"
      "position=< 1,  4> velocity=< 2,  1>"
      "position=<-2,  7> velocity=< 2, -2>"
      "position=< 3,  6> velocity=<-1, -1>"
      "position=< 5,  0> velocity=< 1,  0>"
      "position=<-6,  0> velocity=< 2,  0>"
      "position=< 5,  9> velocity=< 1, -2>"
      "position=<14,  7> velocity=<-2,  0>"
      "position=<-3,  6> velocity=< 2, -1>" ]
    |> List.map parseInput

problem1 testData

let input =
    "../input/day10.txt"
    |> File.ReadAllLines
    |> Array.map parseInput
    |> Array.toList

problem1 input

let ret2 = problem2 input
printfn "problem2 = %d" ret2
