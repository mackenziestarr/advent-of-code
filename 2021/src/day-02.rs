#[derive(Debug)]
struct Answer {
    position: i32,
    depth: i32,
}

fn part_one(input: &Vec<(&str, i32)>) -> i32 {
    let init = Answer {
        position: 0,
        depth: 0,
    };
    let result = input.iter().fold(init, |result, s| match s {
        ("up", x) => Answer {
            depth: result.depth - x,
            ..result
        },
        ("down", x) => Answer {
            depth: result.depth + x,
            ..result
        },
        ("forward", x) => Answer {
            position: result.position + x,
            ..result
        },
        _ => result,
    });
    result.depth * result.position
}

fn main() {
    let input: Vec<(&str, i32)> = include_str!("../input/day-02.txt")
        .lines()
        .map(|s| {
            let v: Vec<&str> = s.splitn(2, " ").collect();
            (v[0], v[1].parse::<i32>().unwrap())
        })
        .collect();

    println!("day two:");
    println!("- part one: {}", part_one(&input));
    println!("- part two: {}", "TODO");
}
