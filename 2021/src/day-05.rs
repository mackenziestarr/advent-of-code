use std::collections::HashMap;
use std::cmp;

#[derive(Debug, Clone)]
struct Point {
    x: i32,
    y: i32,
}

#[derive(Debug, Clone)]
struct Segment {
    start: Point,
    end: Point,
}

impl Segment {
    fn slope(&self) -> f32 {
        (self.end.y - self.start.y) as f32 / (self.end.x - self.start.x) as f32
    }
    fn is_straight_line(&self) -> bool {
        self.start.x == self.end.x || self.start.y == self.end.y
    }
}

fn parse(input: &str) -> Vec<Segment> {
    input.lines()
        .map(|line| {
            // TODO replace with ?
            let (start, end) = line.trim().split_once(" -> ").unwrap();
            let (x1, y1) = start.split_once(',').unwrap();
            let (x2, y2) = end.split_once(',').unwrap();
            Segment {
                start: Point { x: x1.parse().unwrap(), y: y1.parse().unwrap() },
                end: Point { x: x2.parse().unwrap(), y: y2.parse().unwrap() },
            }
        })
        .collect()
}

#[test]
fn test_part_one() {
    let example_input =
        "0,9 -> 5,9
         8,0 -> 0,8
         9,4 -> 3,4
         2,2 -> 2,1
         7,0 -> 7,4
         6,4 -> 2,0
         0,9 -> 2,9
         3,4 -> 1,4
         0,0 -> 8,8
         5,5 -> 8,2";
    let input: Vec<Segment> = parse(example_input);
    assert_eq!(part_one(&input), 5);
}

fn part_one(segments: &[Segment]) -> usize {
    let mut points: HashMap<(i32, i32), usize> = HashMap::new();
    let segments: Vec<Segment> = segments
        .iter()
        .filter(|s| s.is_straight_line())
        .cloned()
        .collect();
    for segment in segments {
        for x in cmp::min(segment.start.x, segment.end.x)..=cmp::max(segment.start.x, segment.end.x) {
            for y in cmp::min(segment.start.y,segment.end.y)..=cmp::max(segment.start.y, segment.end.y) {
                let overlap = points.entry((x, y)).or_default();
                *overlap += 1;
            }
        }
    }
    points.iter().filter(|f| *(*f).1 >= 2).count()
}

#[test]
fn test_part_two() {
    let example_input =
        "0,9 -> 5,9
         8,0 -> 0,8
         9,4 -> 3,4
         2,2 -> 2,1
         7,0 -> 7,4
         6,4 -> 2,0
         0,9 -> 2,9
         3,4 -> 1,4
         0,0 -> 8,8
         5,5 -> 8,2";
    let input: Vec<Segment> = parse(example_input);
    assert_eq!(part_two(&input), 12);
}

fn part_two(segments: &[Segment]) -> usize {
    let mut points: HashMap<(i32, i32), usize> = HashMap::new();
    let segments: Vec<Segment> = segments
        .iter()
        .filter(|segment|
            segment.is_straight_line() || segment.slope().abs() == 1.0f32
        )
        .cloned()
        .collect();
    for segment in segments {
        for x in cmp::min(segment.start.x, segment.end.x)..=cmp::max(segment.start.x, segment.end.x) {
            if segment.slope().abs() == 1.0f32 {
                let m = segment.slope();
                let b = segment.start.y - m as i32 * segment.start.x;
                let y = m as i32 * x + b;
                let overlap = points.entry((x, y)).or_default();
                *overlap += 1;
            } else {
                for y in cmp::min(segment.start.y,segment.end.y)..=cmp::max(segment.start.y, segment.end.y) {
                    let overlap = points.entry((x, y)).or_default();
                    *overlap += 1;
                }
            }
        }
    }
    points.iter().filter(|f| *(*f).1 >= 2).count()
}

fn main() {
    let input = include_str!("../input/day-05.txt");
    let segments = parse(input);
    println!("day five:");
    println!("- part one: {}", part_one(&segments));
    println!("- part two: {}", part_two(&segments));
}