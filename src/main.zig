const std = @import("std");
const sdl = @import("sdl3");

const graphics = @import("graphics.zig");

pub fn main() !void {
    defer sdl.shutdown();
    
    const init_flags = sdl.InitFlags { .video = true, .events = true };
    try sdl.init(init_flags);
    defer sdl.quit(init_flags);

    const window = try sdl.video.Window.init("test", 1280, 720, .{ .open_gl = true });
    defer window.deinit();

    const g = try graphics.Graphics.init(window);
    defer g.deinit() catch @panic("Failed to deinit graphics!");

    var alive = true;
    while (alive) {
        while (sdl.events.poll()) |event| {
            switch (event) {
                .quit => alive = false,
                .window_close_requested => alive = false,
                else => {}
            }
        }

        g.clear(1.0, 0.5, 0.25, 1.0);
        try g.present(1);
    }
}
