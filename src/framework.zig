const sdl = @import("sdl3");

pub const Window = struct {
    _window: sdl.video.Window,

    pub fn init(title: [:0]const u8, width: u32, height: u32) !Window {
        errdefer sdl.shutdown();

        const init_flags = sdl.InitFlags { .video = true, .events = true };
        try sdl.init(init_flags);
        errdefer sdl.quit(init_flags);
        
        const window = try sdl.video.Window.init(title, width, height, .{ .open_gl = true });
        errdefer window.deinit();

        return Window { ._window = window };
    }

    pub fn deinit(self: *const Window) void {
        self._window.deinit();
        sdl.shutdown();
    }

    pub fn handle(self: *const Window) sdl.video.Window {
        return self._window;
    }
};