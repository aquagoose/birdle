const gl = @import("zgl");
const sdl = @import("sdl3");

fn getProcAddress(comptime _: type, symbolName: [:0]const u8) ?*const anyopaque {
    return sdl.video.gl.getProcAddress(symbolName);
}

pub const Graphics = struct {
    window: sdl.video.Window,
    ctx: sdl.video.gl.Context,

    pub fn init(window: sdl.video.Window) !Graphics {
        const ctx = try sdl.video.gl.Context.init(window);

        try gl.loadExtensions(void, getProcAddress);

        return Graphics { .window = window, .ctx = ctx };
    }

    pub fn deinit(self: *const Graphics) !void {
        try self.ctx.deinit();
    }

    pub fn clear(_: *const Graphics, r: f32, g: f32, b: f32, a: f32) void {
        gl.clearColor(r, g, b, a);
        gl.clear(.{ .color = true });
    }

    pub fn present(self: *const Graphics, present_interval: u8) !void {
        try sdl.video.gl.setSwapInterval(@enumFromInt(present_interval));
        try sdl.video.gl.swapWindow(self.window);
    }
};