#[test]
fn test_part_one() {
    let input: Vec<i32> = vec![16, 1, 2, 0, 4, 2, 7, 1, 2, 14];
    assert_eq!(part_one(&input, |n| n), 37)
}

#[test]
fn test_part_two() {
    let input: Vec<i32> = vec![16, 1, 2, 0, 4, 2, 7, 1, 2, 14];
    assert_eq!(part_two(&input, |n| (n * n + n) / 2), 168)
}

fn minimize_cost<F>(mut input: Vec<i32>, cost_fn: F) -> i32
    where F: Fn(i32) -> i32 {
    input.sort();
    let (min, max) = (*input.first().unwrap(), *input.last().unwrap());
    (min..=max).fold(i32::MAX, |cost, i| {
        i32::min(
            cost,
            input.iter().fold(0, |sum, j| {
                let distance: i32 = (i - j).abs();
                sum + cost_fn(distance)
            }),
        )
    })
}

fn main() {
    let input: Vec<i32> = include_str!("../input/day-07.txt")
        .trim()
        .split(',')
        .map(|n| n.parse::<i32>().unwrap_or(0))
        .collect();
    println!("day seven:");
    println!("- part one: {}", minimize_cost(input.clone(), |n| n));
    println!("- part two: {}", minimize_cost(input, |n| (n * n + n) / 2));
}