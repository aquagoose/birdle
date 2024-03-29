﻿using birdle.Graphics;

namespace birdle.GUI.Elements;

public class TextElement : UIElement
{
    private string _text;
    
    public Font Font;

    public string Text
    {
        get => _text;
        set
        {
            _text = value;
            Size = Font.MeasureString(FontSize, value);
        }
    }

    public uint FontSize;

    public TextElement(Position position, string text, uint fontSize) : base(position)
    {
        Font = UI.Font;
        FontSize = fontSize;
        
        Text = text;
    }

    public override void Update(float dt, float scale, ref bool mouseCaptured)
    {
        // Text elements are "mouse transparent", therefore we can't call the base method.
    }

    public override void Draw(SpriteRenderer renderer, float scale)
    {
        Font.Draw(renderer, (uint) (FontSize * scale), Text, WorldPosition, ColorScheme.TextColor);
    }
}