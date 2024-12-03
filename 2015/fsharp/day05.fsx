open System.IO

let inline isVowel (c: char) : bool =
    c = 'a' || c = 'e' || c = 'i' || c = 'o' || c = 'u'

let inline isIllegalWord p1 p2 =
    match p1, p2 with
    | 'a', 'b'
    | 'c', 'd'
    | 'p', 'q'
    | 'x', 'y' -> true
    | _ -> false

let isNiceString (s: string) : bool =
    let rec f cs prev vowels doubles =
        match cs with
        | [] -> vowels >= 3 && doubles >= 1
        | h :: t ->
            let vowels = if isVowel h then vowels + 1 else vowels

            if h = prev then f t h vowels (doubles + 1)
            else if isIllegalWord prev h then false
            else f t h vowels doubles

    f (Seq.toList s) '?' 0 0

// true
isNiceString "ugknbfddgicrmopn"

// true
isNiceString "aaa"

// false,false,false
[ "jchzalrnumimnmhp"; "haegwjzuvuyypxyu"; "dvszwmarrgswjxmb" ]
|> List.map isNiceString

let hasDoublePair (s: string) : bool =
    let rec hasDoublePair' pairs =
        match pairs with
        | []
        | _ :: []
        | _ :: _ :: [] -> false
        | h :: _ :: t ->
            if List.exists ((=) h) t then
                true
            else
                hasDoublePair' (List.tail pairs)

    hasDoublePair' (s |> Seq.toList |> List.windowed 2)

let hasLetterWithOneBetween (s: string) : bool =
    let rec hasLetterWithOneBetween' cs =
        match cs with
        | []
        | _ :: []
        | _ :: _ :: [] -> false
        | h1 :: _ :: h3 :: _ ->
            if h1 = h3 then
                true
            else
                hasLetterWithOneBetween' (List.tail cs)

    hasLetterWithOneBetween' (Seq.toList s)

let isNiceString2 (s: string) : bool =
    hasDoublePair s && hasLetterWithOneBetween s

// true
isNiceString2 "qjhvhtzxzqqjkmpb"

// true
isNiceString2 "xxyxx"

// false
isNiceString2 "uurcxstgmygtbstg"

// false
isNiceString2 "ieodomkazucvgmuy"

let input = File.ReadLines("../input/day05.txt") |> Seq.toList
let problem1 = input |> List.filter isNiceString |> List.length
// 258
printfn "problem1: %d" problem1

let problem2 = input |> List.filter isNiceString2 |> List.length
printfn "problem2: %d" problem2
