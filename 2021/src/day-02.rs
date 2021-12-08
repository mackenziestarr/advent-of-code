#[derive(Debug)]
struct Answer {
    aim: i32,
    position: i32,
    depth: i32,
}

fn part_one(input: &[(&str, i32)]) -> i32 {
    let init = Answer {
        aim: 0,
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

fn part_two(input: &[(&str, i32)]) -> i32 {
    let init = Answer {
        aim: 0,
        position: 0,
        depth: 0,
    };
    let result = input.iter().fold(init, |result, s| match s {
        ("up", x) => Answer {
            aim: result.aim - x,
            ..result
        },
        ("down", x) => Answer {
            aim: result.aim + x,
            ..result
        },
        ("forward", x) => Answer {
            position: result.position + x,
            depth: result.depth + (result.aim * x),
            ..result
        },
        _ => result,
    });
    result.depth * result.position
}

#[test]
fn test_part_two() {
    let input = vec![
        ("forward", 5),
        ("down", 5),
        ("forward", 8),
        ("up", 3),
        ("down", 8),
        ("forward", 2),
    ];
    assert_eq!(part_two(&input), 900)
}

fn main() {
    let input: Vec<(&str, i32)> = include_str!("../input/day-02.txt")
        .lines()
        .map(|s| {
            let v: Vec<&str> = s.splitn(2, ' ').collect();
            (v[0], v[1].parse::<i32>().unwrap())
        })
        .collect();

    println!("day two:");
    println!("- part one: {}", part_one(&input));
    println!("- part two: {}", part_two(&input));
}
