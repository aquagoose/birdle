using System;
using System.Collections.Generic;
using System.Drawing;
using System.Numerics;
using Pie;
using Pie.Text;

namespace birdle.Graphics;

public class Font : IDisposable
{
    private static FreeType _freeType;

    private GraphicsDevice _device;
    
    private Face _face;

    private Dictionary<uint, Dictionary<char, Character>> _cachedCharacters;

    static Font()
    {
        _freeType = new FreeType();
    }
    
    public Font(GraphicsDevice device, string path)
    {
        _device = device;
        
        _face = _freeType.CreateFace(path);
        _cachedCharacters = new Dictionary<uint, Dictionary<char, Character>>();
    }

    public void Draw(SpriteRenderer renderer, uint size, string text, Vector2 position, Color color)
    {
        Vector2 currentPosition = position;
        
        GetCharSet(size, text, out Dictionary<char, Character> characters, out int largestChar);

        currentPosition.Y += largestChar;

        foreach (char c in text)
        {
            Character character = characters[c];
            
            switch (c)
            {
                case ' ':
                    currentPosition.X += character.Advance;
                    continue;

                case '\n':
                    currentPosition.Y += size;
                    currentPosition.X = position.X;
                    continue;
            }

            renderer.Draw(character.Texture, currentPosition + new Vector2(character.Bearing.X, -character.Bearing.Y),
                color, 0, Vector2.One, Vector2.Zero);
            currentPosition.X += character.Advance;
        }
    }

    public Size MeasureString(uint size, string text)
    {
        int currentX = 0;
        
        GetCharSet(size, text, out Dictionary<char, Character> characters, out int largestChar);
        Size finalSize = new Size(0, largestChar);
        
        foreach (char c in text)
        {
            switch (c)
            {
                case '\n':
                    currentX = 0;
                    finalSize.Height += (int) size;
                    continue;
            }
            
            currentX += characters[c].Advance;

            if (currentX > finalSize.Width)
                finalSize.Width = currentX;
        }

        return finalSize;
    }

    public void DisposeTextureCache()
    {
        BirdleGame.Log(LogType.Debug, "Disposing texture cache...");
        
        foreach ((_, Dictionary<char, Character> charSet) in _cachedCharacters)
        {
            foreach ((_, Character character) in charSet)
                character.Texture?.Dispose();
            
            charSet.Clear();
        }
        
        _cachedCharacters.Clear();
        
        BirdleGame.Log(LogType.Debug, "Texture cache disposed.");
    }

    private void GetCharSet(uint size, string text, out Dictionary<char, Character> characters, out int largestChar)
    {
        largestChar = 0;
        
        if (!_cachedCharacters.TryGetValue(size, out characters))
        {
            characters = new Dictionary<char, Character>();
            _cachedCharacters.Add(size, characters);
        }

        foreach (char c in text)
        {
            if (!characters.TryGetValue(c, out Character character))
            {
                BirdleGame.Log(LogType.Verbose, $"Creating character '{c}' at size {size}.");
                
                Pie.Text.Character ftChar = _face.GetCharacter(c, size);

                Size charSize = new Size(ftChar.Width, ftChar.Height);
                Vector2 bearing = new Vector2(ftChar.BitmapLeft, ftChar.BitmapTop);

                Texture texture = null;
                
                if (charSize != Size.Empty) 
                    texture = _device.CreateTexture(charSize, ftChar.Bitmap);

                character = new Character()
                {
                    Texture = texture,
                    Size = charSize,
                    Bearing = bearing,
                    Advance = ftChar.Advance
                };
                
                characters.Add(c, character);
            }

            if (character.Size.Height > largestChar)
                largestChar = character.Size.Height;
        }
    }
    
    public void Dispose()
    {
        _face.Dispose();
    }

    private struct Character
    {
        public Texture Texture;
        public Size Size;
        public Vector2 Bearing;
        public int Advance;
    }
}