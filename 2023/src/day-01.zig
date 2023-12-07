const std = @import("std");
const input = @embedFile("day-01.txt");
pub fn main() !void {
    std.debug.print("part one answer: {d}\n", .{try partOne(input)});
    std.debug.print("part two answer: {d}\n", .{try partTwo(input)});
}

fn partOne(in: []const u8) !u32 {
    var lines = std.mem.tokenize(u8, in, "\n");
    var sum: u32 = 0;
    while (lines.next()) |line| sum += try collect(line);
    return sum;
}

fn partTwo(in: []const u8) !u32 {
    var lines = std.mem.tokenize(u8, in, "\n");
    var sum: u32 = 0;
    while (lines.next()) |line| {
        const replacedLine = try replace(line);
        sum += try collect(replacedLine);
    }
    return sum;
}

const Error = error{InvalidInput};
fn collect(line: []const u8) !u32 {
    var start: usize = 0;
    var end: usize = line.len - 1;
    while (start <= end) {
        const startIsDigit = std.ascii.isDigit(line[start]);
        const endIsDigit = std.ascii.isDigit(line[end]);
        if (!startIsDigit) start += 1;
        if (!endIsDigit) end -= 1;
        if (startIsDigit and endIsDigit) {
            return std.fmt.parseUnsigned(u32, &[2]u8{ line[start], line[end] }, 10);
        }
    }
    return Error.InvalidInput;
}

const words = [_][]const u8{ "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };
const numbers = [_][]const u8{ "1", "2", "3", "4", "5", "6", "7", "8", "9" };

fn replace(line: []const u8) ![]u8 {
    const allocator = std.heap.page_allocator;
    var out: []u8 = try allocator.dupe(u8, line);
    for (words, 0..) |word, index| {
        const number = numbers[index];
        const token = try std.fmt.allocPrint(allocator, "{s}{s}{s}{s}{s}", .{ word, number, word, number, word });
        const size = std.mem.replacementSize(u8, out, word, token);
        const buf = try allocator.alloc(u8, size);
        _ = std.mem.replace(u8, out, word, token, buf);
        out = buf;
    }
    return out;
}

test "collect" {
    try std.testing.expectEqual(collect("treb7uchet"), 77);
    try std.testing.expectEqual(collect("1abc2"), 12);
    try std.testing.expectError(Error.InvalidInput, collect("abcd"));
}

test "replace" {
    const actual = try replace("oneight");
    try std.testing.expectEqualStrings("one1one1oneight8eight8eight", actual);
}

test "part two" {
    try std.testing.expectEqual(collect(try replace("two1nine")), 29);
    try std.testing.expectEqual(collect(try replace("eightwothree")), 83);
    try std.testing.expectEqual(collect(try replace("abcone2threexyz")), 13);
    try std.testing.expectEqual(collect(try replace("xtwone3four")), 24);
    try std.testing.expectEqual(collect(try replace("4nineeightseven2")), 42);
    try std.testing.expectEqual(collect(try replace("zoneight234")), 14);
    try std.testing.expectEqual(collect(try replace("7pqrstsixteen")), 76);
}
