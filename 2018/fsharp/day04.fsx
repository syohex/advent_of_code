open System
open System.IO
open System.Text.RegularExpressions

type Action =
    | Sleep
    | WakeUp
    | Guard of int

type Record = { Timestamp: DateTime; Action: Action }

let matchToDateTime (m: Match) : DateTime =
    let year, month, day, hour, minute =
        int m.Groups.["year"].Value,
        int m.Groups.["month"].Value,
        int m.Groups.["day"].Value,
        int m.Groups.["hour"].Value,
        int m.Groups.["minute"].Value

    DateTime(year, month, day, hour, minute, 0)

let parseLine (s: string) : Record =
    let timeStampPattern =
        @"\[(?<year>\d+)-(?<month>\d+)-(?<day>\d+) (?<hour>\d+):(?<minute>\d+)\]"

    if s.Contains("begins") then
        let pattern = sprintf @"%s Guard #(?<id>\d+)" timeStampPattern
        let m = Regex.Match(s, pattern)

        if m.Success then
            { Timestamp = matchToDateTime m
              Action = Guard(int m.Groups.["id"].Value) }
        else
            failwithf "cannot parse Guard line: %s" s
    elif s.Contains("falls asleep") then
        let m = Regex.Match(s, timeStampPattern)

        { Timestamp = matchToDateTime m
          Action = Sleep }
    elif s.Contains("wakes up") then
        let m = Regex.Match(s, timeStampPattern)

        { Timestamp = matchToDateTime m
          Action = WakeUp }
    else
        failwithf "unknown line: %s" s

let totalSleepMinutes (times: (DateTime * DateTime) list) : int =
    times |> List.map (fun (sleep, awake) -> awake - sleep |> _.Minutes) |> List.sum

let collectSleepTimes (input: Record list) : Map<int, (DateTime * DateTime) list> =
    let rec f input currentGuard sleepTime acc =
        match input with
        | [] -> acc
        | h :: t ->
            match h.Action with
            | Guard id_ -> f t id_ sleepTime acc
            | Sleep -> f t currentGuard h.Timestamp acc
            | WakeUp ->
                let v = Map.tryFind currentGuard acc |> Option.defaultValue []
                let acc = Map.add currentGuard ((sleepTime, h.Timestamp) :: v) acc
                f t currentGuard h.Timestamp acc

    f input -1 DateTime.Now Map.empty

let toMinutesFreq (input: (DateTime * DateTime) list) : int[] =
    input
    |> List.fold
        (fun (acc: int[]) (sleep, awake) ->
            seq { sleep.Minute .. (awake.Minute - 1) }
            |> Seq.iter (fun i -> acc.[i] <- acc.[i] + 1)

            acc)
        (Array.zeroCreate 60)

let maxFreqMinutes (freq: int[]) : int * int =
    freq
    |> Array.indexed
    |> Array.fold (fun (maxMinute, maxFreq) (i, v) -> if v > maxFreq then i, v else maxMinute, maxFreq) (-1, -1)

let problem1 (input: Record list) : int =
    let sleeps = collectSleepTimes input

    let maxId, maxTimes, _ =
        sleeps
        |> Map.fold
            (fun (maxId, maxTimes, maxTotal) id_ times ->
                let total = totalSleepMinutes times

                if total > maxTotal then
                    id_, times, total
                else
                    maxId, maxTimes, maxTotal)
            (-1, [], 0)

    let minutesFreq = toMinutesFreq maxTimes
    let maxMinutes, _ = maxFreqMinutes minutesFreq

    maxId * maxMinutes


let problem2 (input: Record list) : int =
    let sleeps = collectSleepTimes input

    let maxId, maxFreqMin, _ =
        sleeps
        |> Map.fold
            (fun (maxId, maxFreqMin, maxFreq) id_ times ->
                let freq = toMinutesFreq times
                let minutes, freq = maxFreqMinutes freq

                if freq > maxFreq then
                    id_, minutes, freq
                else
                    maxId, maxFreqMin, maxFreq)
            (-1, -1, -1)

    maxId * maxFreqMin

let testData1 =
    [ "[1518-11-01 00:00] Guard #10 begins shift"
      "[1518-11-01 00:05] falls asleep"
      "[1518-11-01 00:25] wakes up"
      "[1518-11-01 00:30] falls asleep"
      "[1518-11-01 00:55] wakes up"
      "[1518-11-01 23:58] Guard #99 begins shift"
      "[1518-11-02 00:40] falls asleep"
      "[1518-11-02 00:50] wakes up"
      "[1518-11-03 00:05] Guard #10 begins shift"
      "[1518-11-03 00:24] falls asleep"
      "[1518-11-03 00:29] wakes up"
      "[1518-11-04 00:02] Guard #99 begins shift"
      "[1518-11-04 00:36] falls asleep"
      "[1518-11-04 00:46] wakes up"
      "[1518-11-05 00:03] Guard #99 begins shift"
      "[1518-11-05 00:45] falls asleep"
      "[1518-11-05 00:55] wakes up" ]

let testRecords1 = testData1 |> List.map parseLine
// 240
problem1 testRecords1
// 4455
problem2 testRecords1

let input = "../input/day04.txt" |> File.ReadLines |> Seq.toList
let records = input |> List.map parseLine |> List.sortBy _.Timestamp

let ret1 = problem1 records
let ret2 = problem2 records

printfn "problem1 = %d" ret1
printfn "problem2 = %d" ret2
