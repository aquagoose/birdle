const std = @import("std");

const sdl = @import("sdl3");

const framework = @import("framework.zig");
const render = @import("render.zig");

pub const Game = struct {
    window: framework.Window,
    graphics: render.Graphics,
    
    pub fn init() !Game {
        const window = try framework.Window.init("birdle", 800, 600);
        const graphics = try render.Graphics.init(&window);

        //const allocator = std.heap.DebugAllocator(.{}).init;
        //const renderer = try render.SpriteRenderer.init(allocator);
        //allocator.

        return Game { .window = window, .graphics = graphics };
    }

    pub fn deinit(self: *const Game) void {
        self.graphics.deinit() catch @panic("Failed to deinit graphics!");
        self.window.deinit();
    }

    pub fn run(self: *const Game) void {
        var alive = true;
        while (alive) {
            while (sdl.events.poll()) |event| {
                switch (event) {
                    .quit => alive = false,
                    .window_close_requested => alive = false,
                    else => {}
                }
            }
            
            self.graphics.clear(1.0, 0.5, 0.25, 1.0);
            self.graphics.present(1) catch {};
        }
    }
};