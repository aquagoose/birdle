using System.Drawing;

namespace birdle.GUI;

public struct ColorScheme
{
    public Color BackgroundColor;

    public Color TextColor;

    public Color GoodColor;

    public Color AlmostColor;

    public Color BadColor;

    public Color EmptyColor;

    public Color OopsColor;

    public Color BorderColor;

    public Color ClickColor;

    public static ColorScheme Default => new ColorScheme()
    {
        BackgroundColor = Color.Bisque,
        TextColor = Color.Black,
        
        GoodColor = Color.LimeGreen,
        AlmostColor = Color.Gold,
        BadColor = Color.Lavender,
        
        // Bisque but darker
        EmptyColor = Color.FromArgb(230, 205, 176),
        
        OopsColor = Color.IndianRed,
        
        BorderColor = Color.Gray,
        
        ClickColor = Color.Beige
    };

    public static ColorScheme Dark => new ColorScheme()
    {
        BackgroundColor = Color.FromArgb(26, 26, 26),
        TextColor = Color.White,
        
        GoodColor = Color.DarkGreen,
        AlmostColor = Color.DarkGoldenrod,
        BadColor = Color.FromArgb(70, 70, 70),
        
        EmptyColor = Color.FromArgb(32, 32, 32),
        
        OopsColor = Color.DarkRed,
        
        BorderColor = Color.Gray,
        
        ClickColor = Color.FromArgb(60, 60, 60)
    };
}