open System.IO
open System.Text.RegularExpressions

type Reindeer =
    { Name: string
      Velocity: int
      FlyingSeconds: int
      RestingSeconds: int }

let parse (input: string list) : Reindeer list =
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

let afterNSeconds (seconds: int) (r: Reindeer) : int =
    let rec afterNSeconds' seconds isFlying (r: Reindeer) acc =
        if seconds <= 0 then
            acc
        else
            let actionTime =
                if isFlying then
                    min seconds r.FlyingSeconds
                else
                    min seconds r.RestingSeconds

            if isFlying then
                afterNSeconds' (seconds - actionTime) false r (acc + r.Velocity * actionTime)
            else
                afterNSeconds' (seconds - actionTime) true r acc

    afterNSeconds' seconds true r 0

let problem1 (seconds: int) (input: Reindeer list) : int =
    input
    |> List.map (fun d -> afterNSeconds seconds d)
    |> List.sortDescending
    |> List.head

type State =
    | Flying of int
    | Resting of int

let updateState (d: Reindeer) ((state, distance): State * int) : State * int =
    match state with
    | Flying n ->
        let distance = distance + d.Velocity

        if n = 1 then
            Resting d.RestingSeconds, distance
        else
            Flying(n - 1), distance
    | Resting n ->
        if n = 1 then
            Flying d.FlyingSeconds, distance
        else
            Resting(n - 1), distance

let givePoint (winners: Reindeer seq) (points: Map<string, int>) : Map<string, int> =
    winners
    |> Seq.fold
        (fun acc w ->
            let v = Map.tryFind w.Name acc |> Option.defaultValue 0
            Map.add w.Name (v + 1) acc)
        points

let rec doRace (seconds: int) (states: Map<Reindeer, State * int>) (points: Map<string, int>) : int =
    if seconds = 0 then
        points |> Map.values |> Seq.max
    else
        let states = states |> Map.map (fun r state -> updateState r state)
        let maxDistance = states |> Map.fold (fun acc _ (_, distance) -> max acc distance) 0

        let winners =
            states |> Map.filter (fun _ (_, distance) -> distance = maxDistance) |> Map.keys

        let points = givePoint winners points

        doRace (seconds - 1) states points


let problem2 (seconds: int) (input: Reindeer list) : int =
    let states =
        input
        |> List.fold (fun acc r -> Map.add r (Flying r.FlyingSeconds, 0) acc) Map.empty

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
