open System.IO
open System.Text.RegularExpressions

type Data =
    { Name: string
      Velocity: int
      FlyingSeconds: int
      RestingSeconds: int }

let parse (input: string list) : Data list =
    let re =
        new Regex(@"(\S+) can fly (\S+) km/s for (\S+) seconds, but then must rest for (\S+) seconds.")

    input
    |> List.map (fun s ->
        let m = re.Match(s)

        if m.Success then
            { Name = m.Groups.[1].Value
              Velocity = int m.Groups.[2].Value
              FlyingSeconds = int m.Groups.[3].Value
              RestingSeconds = int m.Groups.[4].Value }
        else
            failwithf "invalid input %s" s)

let afterNSeconds (seconds: int) (d: Data) : int =
    let rec afterNSeconds' seconds isFlying (d: Data) acc =
        if seconds <= 0 then
            acc
        else
            let actionTime =
                if isFlying then
                    min seconds d.FlyingSeconds
                else
                    min seconds d.RestingSeconds

            if isFlying then
                afterNSeconds' (seconds - actionTime) false d (acc + d.Velocity * actionTime)
            else
                afterNSeconds' (seconds - actionTime) true d acc

    afterNSeconds' seconds true d 0

let problem1 (seconds: int) (input: Data list) : int =
    input
    |> List.map (fun d -> afterNSeconds seconds d)
    |> List.sortDescending
    |> List.head

type State =
    | Flying of int
    | Resting of int

let updateState ((d, current, distance): Data * State * int) : Data * State * int =
    match current with
    | Flying n ->
        let distance = distance + d.Velocity

        if n = 1 then
            d, Resting d.RestingSeconds, distance
        else
            d, Flying(n - 1), distance
    | Resting n ->
        if n = 1 then
            d, Flying d.FlyingSeconds, distance
        else
            d, Resting(n - 1), distance

let rec doRace (seconds: int) (states: (Data * State * int) list) (points: Map<string, int>) : int =
    if seconds = 0 then
        points |> Map.values |> Seq.max
    else
        let states = states |> List.map (fun state -> updateState state)
        let _, _, maxDistance = states |> List.maxBy (fun (_, _, distance) -> distance)

        let winners =
            states
            |> List.fold (fun acc (d, _, distance) -> if distance = maxDistance then d :: acc else acc) []

        let points =
            winners
            |> List.fold
                (fun acc winner ->
                    match Map.tryFind winner.Name acc with
                    | Some(v) -> Map.add winner.Name (v + 1) acc
                    | None -> Map.add winner.Name 1 acc)
                points

        doRace (seconds - 1) states points


let problem2 (seconds: int) (input: Data list) : int =
    let states = input |> List.map (fun d -> d, Flying d.FlyingSeconds, 0)
    doRace seconds states Map.empty

let testInput =
    [ "Comet can fly 14 km/s for 10 seconds, but then must rest for 127 seconds."
      "Dancer can fly 16 km/s for 11 seconds, but then must rest for 162 seconds." ]

let testData = parse testInput
// 1120
problem1 1000 testData
// 689
problem2 1000 testData

let input = File.ReadAllLines("../input/day14.txt") |> Array.toList
let data = parse input

let ret1 = problem1 2503 data
let ret2 = problem2 2503 data

// 2693
printfn "ret1 = %d" ret1
// 1084
printfn "ret2 = %d" ret2
