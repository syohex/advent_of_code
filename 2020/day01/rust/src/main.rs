use std::collections::HashSet;
use std::io::{self, Read};

fn solve01(nums: &Vec<i32>) -> i32 {
    let hs: HashSet<&i32> = nums.iter().collect();
    for num in nums.iter() {
        let diff = 2020 - num;
        if hs.contains(&diff) {
            return num * diff;
        }
    }

    panic!("never reash here");
}

fn parse_input(input: &str) -> Vec<i32> {
    input.lines().filter_map(|s| s.parse().ok()).collect()
}

fn main() -> io::Result<()> {
    let mut input = String::new();
    io::stdin().read_to_string(&mut input)?;
    let nums = parse_input(&input);

    println!("Part01: {}", solve01(&nums));

    Ok(())
}

#[test]
fn test01() {
    let input = r#"1721
979
366
299
675
456"#;

    let nums = parse_input(input);
    assert_eq!(nums, vec![1721, 979, 366, 299, 675, 456]);
    assert_eq!(solve01(&nums), 514579);
}
