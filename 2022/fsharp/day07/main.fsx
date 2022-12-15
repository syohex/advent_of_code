open System

type File =
    | FileEntry of string * int
    | DirectoryEntry of string

let parseInput (file: string) =
    let rec parseInput' (lines: string list) path (dirs: Map<string list, File list>) =
        match lines with
        | [] -> dirs
        | h :: t ->
            if h.StartsWith("$ cd") then
                let dir = h.Substring(5)

                let nextPath =
                    match dir with
                    | "/" -> [ "/" ]
                    | ".." -> List.tail path
                    | p -> p :: path

                match Map.tryFind nextPath dirs with
                | None -> parseInput' t nextPath (Map.add nextPath [] dirs)
                | Some(_) -> parseInput' t nextPath dirs
            elif h.StartsWith("$ ls") then
                parseInput' t path dirs
            elif h.StartsWith("dir") then
                let name = h.Substring(4)

                match Map.tryFind path dirs with
                | Some(files) ->
                    let files' = DirectoryEntry(name) :: files
                    parseInput' t path (Map.add path files' dirs)
                | _ -> failwithf "could not find directory1 %A" path
            else
                let parts = h.Split [| ' ' |]
                let size, name = parts.[0] |> int, parts.[1]

                match Map.tryFind path dirs with
                | Some(files) ->
                    let files' = FileEntry(name, size) :: files
                    parseInput' t path (Map.add path files' dirs)
                | _ -> failwithf "could not find directory2 %A" path

    let lines = file |> IO.File.ReadLines |> Seq.toList
    parseInput' lines [] Map.empty

let rec fileSize (parent: string list) (file: File) (dirs: Map<string list, File list>) : int =
    match file with
    | FileEntry(_, size) -> size
    | DirectoryEntry(name) ->
        let dir = name :: parent

        match Map.tryFind dir dirs with
        | Some(files) -> files |> List.map (fun f -> fileSize dir f dirs) |> List.sum
        | _ -> failwithf "never reach here %A" dir

let problem1 (dirs: Map<string list, File list>) : int =
    dirs
    |> Map.map (fun parent files -> files |> List.map (fun f -> fileSize parent f dirs) |> List.sum)
    |> Map.values
    |> Seq.filter (fun n -> n < 100000)
    |> Seq.sum

let problem2 (dirs: Map<string list, File list>) : int =
    let used =
        Map.find [ "/" ] dirs |> List.fold (fun acc f -> acc + fileSize [ "/" ] f dirs) 0

    let needed = 30000000 - (70000000 - used)

    dirs
    |> Map.map (fun parent files -> files |> List.map (fun f -> fileSize parent f dirs) |> List.sum)
    |> Map.values
    |> Seq.filter (fun n -> n >= needed)
    |> Seq.min

let testData = parseInput "test.txt"
// 95437
problem1 testData
// 24933642
problem2 testData

let data = parseInput "input.txt"
// 1447046
problem1 data
// 578710
problem2 data
