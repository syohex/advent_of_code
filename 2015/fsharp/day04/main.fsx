open System.Security
open System.Text
open System

let md5OfString (s: string) : string =
    use md5 = Cryptography.MD5.Create()
    let bs = Encoding.ASCII.GetBytes(s)
    let hash = md5.ComputeHash(bs)
    Convert.ToHexString(hash)

let problem1 (secret: string) (prefix: string) : int =
    let rec f n =
        let input = secret + string n
        let md5 = md5OfString input
        if md5.StartsWith(prefix) then n else f (n + 1)

    f 0

// 609043
problem1 "abcdef" "00000"

// 1048970
problem1 "pqrstuv" "00000"

// 282749
problem1 "yzbqklnj" "00000"

problem1 "yzbqklnj" "000000"
