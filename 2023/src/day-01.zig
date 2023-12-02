const std = @import("std");
const input = @embedFile("day-01.txt");
pub fn main() !void {
    var lines = std.mem.tokenize(u8, input, "\n");
    var sum: u32 = 0;
    while (lines.next()) |line| sum += try collect(line);
    std.debug.print("part one answer: {d}\n", .{sum});
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

test "collect" {
    try std.testing.expectEqual(collect("treb7uchet"), 77);
    try std.testing.expectEqual(collect("1abc2"), 12);
    try std.testing.expectError(Error.InvalidInput, collect("abcd"));
}
