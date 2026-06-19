const std = @import("std");

const zopengl = @import("zopengl");
const gl = zopengl.bindings;
const sdl = @import("sdl3");

const math = @import("math");
const Window = @import("framework.zig").Window;

const MAX_SPRITES = 4096;
const NUM_VERTICES = 4;
const NUM_INDICES = 6;
const MAX_VERTICES = NUM_VERTICES * MAX_SPRITES;
const MAX_INDICES = NUM_INDICES * MAX_SPRITES;

fn getProcAddress(name: [*:0]const u8) callconv(.c) ?*const anyopaque {
    return sdl.video.gl.getProcAddress(std.mem.span(name));
}

pub const Graphics = struct {
    _window: sdl.video.Window,
    _ctx: sdl.video.gl.Context,

    pub fn init(window: *const Window) !Graphics {
        const window_handle = window.handle();

        const ctx = try sdl.video.gl.Context.init(window_handle);

        try zopengl.loadCoreProfile(getProcAddress, 3, 3);

        return Graphics { ._window = window_handle, ._ctx = ctx };
    }

    pub fn deinit(self: *const Graphics) !void {
        try self._ctx.deinit();
    }

    pub fn clear(_: *const Graphics, r: f32, g: f32, b: f32, a: f32) void {
        gl.clearColor(r, g, b, a);
        gl.clear(gl.COLOR_BUFFER_BIT);
    }

    pub fn present(self: *const Graphics, present_interval: u8) !void {
        try sdl.video.gl.setSwapInterval(@enumFromInt(present_interval));
        try sdl.video.gl.swapWindow(self._window);
    }
};

pub const SpriteRenderer = struct {
    _allocator: std.mem.Allocator,
    _vertices: []SpriteVertex,
    _indices: []u32,

    pub fn init(allocator: std.mem.Allocator) !SpriteRenderer {
        const vertices = try allocator.alloc(SpriteVertex, MAX_VERTICES);
        const indices = try allocator.alloc(u32, MAX_INDICES);

        return SpriteRenderer {
            ._allocator = allocator,
            ._vertices = vertices,
            ._indices = indices 
        };
    }

    pub fn deinit(self: *const SpriteRenderer) void {
        self._allocator.free(self._indices);
        self._allocator.free(self._vertices);
    }
};

const SpriteVertex = struct {
    position: math.Vec2,
    tex_coord: math.Vec2,
    tint: math.Color 
};