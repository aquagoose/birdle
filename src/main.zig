const std = @import("std");
const sdl = @import("sdl3");

pub fn main() !void {
    defer sdl.shutdown();
    
    const init_flags = sdl.InitFlags { .video = true, .events = true };
    try sdl.init(init_flags);
    defer sdl.quit(init_flags);

    const window = try sdl.video.Window.init("test", 1280, 720, .{ .open_gl = true });
    defer window.deinit();

    const device = try sdl.gpu.Device.init(.{ .spirv = true }, true, null);
    defer device.deinit();

    try device.claimWindow(window);
    defer device.releaseWindow(window);

    var alive = true;
    while (alive) {
        while (sdl.events.poll()) |event| {
            switch (event) {
                .quit => alive = false,
                .window_close_requested => alive = false,
                else => {}
            }
        }

        const cb = try device.acquireCommandBuffer();
        const swapchain_texture, _, _ = try cb.acquireSwapchainTexture(window);

        if (swapchain_texture) |texture| {
            const color_target = sdl.gpu.ColorTargetInfo {
                .texture = texture,
                .clear_color = .{.r = 1.0, .g = 0.5, .b = 0.25, .a = 1.0 },
                .load = .clear,
                .store = .store
            };

            const pass = cb.beginRenderPass(&.{color_target}, null);
            defer pass.end();
        }

        try cb.submit();
    }
}
