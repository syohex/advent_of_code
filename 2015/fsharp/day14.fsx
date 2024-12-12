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

let testInput =
    [ "Comet can fly 14 km/s for 10 seconds, but then must rest for 127 seconds."
      "Dancer can fly 16 km/s for 11 seconds, but then must rest for 162 seconds." ]

let testData = parse testInput
// 1120
problem1 1000 testData

let input = File.ReadAllLines("../input/day14.txt") |> Array.toList
let data = parse input

let ret1 = problem1 2503 data

printfn "ret1 = %d" ret1
