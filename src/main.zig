const game = @import("game.zig");

pub fn main() !void {
    const g = try game.Game.init();
    defer g.deinit();
    g.run();
}
