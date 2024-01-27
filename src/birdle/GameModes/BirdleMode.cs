﻿using System;
using System.Collections.Generic;
using System.IO;
using System.Numerics;
using birdle.GUI;
using birdle.GUI.Elements;
using Pie;
using Pie.Windowing;

namespace birdle.GameModes;

public class BirdleMode : GameMode
{
    private BirdleGrid _grid;
    private TextElement _temp;

    private int _currentColumn;
    private int _currentRow;

    private string _word;

    private float _timer;
    private bool _flash;

    public override void Initialize()
    {
        base.Initialize();

        _grid = new BirdleGrid(BirdleGame.UI, new Position(Anchor.TopCenter, new Vector2(0, 20)), 6, 5, 50, 5, 40);

        _temp = new TextElement(BirdleGame.UI, new Position(Anchor.MiddleCenter), "Nice.", 200);

        List<string> words = new List<string>();

        using StreamReader reader = File.OpenText("Content/wordrepo.txt");
        string word;
        while ((word = reader.ReadLine()) != null)
            words.Add(word);
        
        _word = words[Random.Shared.Next(words.Count)].ToUpper();
        
        BirdleGame.UI.AddElement(_grid);
        
        BirdleGame.TextInput += BirdleGameOnTextInput;
        BirdleGame.KeyDown += BirdleGameOnKeyDown;
    }

    private void BirdleGameOnKeyDown(Key key, bool repeat)
    {
        switch (key)
        {
            case Key.Enter:
                CheckWord();
                break;
            
            case Key.Backspace:
                if (_currentColumn <= 0)
                    break;

                _currentColumn--;
                _grid.Slots[_currentColumn, _currentRow].Character = char.MinValue;
                
                break;
        }
    }

    private void BirdleGameOnTextInput(char c)
    {
        if (_currentColumn >= _grid.Columns)
            return;

        c = char.ToUpper(c);
        
        BirdleGame.Log(LogType.Debug, $"_grid.Slots[{_currentColumn}, {_currentRow}].Character = '{c}'");

        _grid.Slots[_currentColumn, _currentRow].Character = c;

        _currentColumn++;
    }

    private void CheckWord()
    {
        if (_currentColumn < _grid.Columns)
        {
            _flash = true;
            return;
        }
        
        int numCorrect = 0;
        
        for (int i = 0; i < _grid.Columns; i++)
        {
            BirdleGame.Log(LogType.Debug, $"Process column {i}");
            
            ref BirdleGrid.Slot slot = ref _grid.Slots[i, _currentRow];

            if (_word[i] == slot.Character)
            {
                slot.State = BirdleGrid.SlotState.Good;
                numCorrect++;
            }
            else if (_word.Contains(slot.Character))
            {
                // This algorithm works out if a character should be displayed as "almost", or "bad".
                // Say a word has 2 occurrences of the letter 'a', and the user's guess has 3 occurrences of 'a', one
                // of which is in the correct slot. Because of this, we still display as having an 'a' available, and
                // even if the entire guess was 'a's, all occurrences would show up as "almost" (to not give it away).
                // However, if the user gets both 'a's in the right slots, the game will no longer show any available
                // 'a's, and will instead mark all other 'a's as "bad", to show that there are no more a's available.
                // Does this make sense? This feels like a horrible explanation...
                
                // This algorithm takes the very naive approach of counting occurrences in the word, then counting the
                // number of CORRECT occurrences in the guess (as in - the guess slot matches the word). If the count
                // matches, then we know there are no more of that letter available, so show all other occurrences as
                // "bad", otherwise show them as "almost".
                
                int numOccurrencesInWord = 0;
                foreach (char c in _word)
                {
                    if (c == slot.Character)
                        numOccurrencesInWord++;
                }

                int numOccurrencesInGuess = 0;
                for (int j = 0; j < _grid.Columns; j++)
                {
                    if (_grid.Slots[j, _currentRow].Character == slot.Character &&
                        _grid.Slots[j, _currentRow].Character == _word[j])
                        numOccurrencesInGuess++;
                }

                if (numOccurrencesInGuess == numOccurrencesInWord)
                    slot.State = BirdleGrid.SlotState.Bad;
                else
                    slot.State = BirdleGrid.SlotState.Almost;
                
                BirdleGame.Log(LogType.Debug, $"word: {numOccurrencesInWord} guess: {numOccurrencesInGuess}");
            }
            else
                slot.State = BirdleGrid.SlotState.Bad;
        }

        if (numCorrect == _word.Length)
            BirdleGame.UI.AddElement(_temp);
        else
        {
            _currentRow++;

            if (_currentRow >= _grid.Rows)
            {
                _temp.FontSize = 50;
                _temp.Text = $"Oof. The word was \"{_word.ToLower()}\".";
                
                BirdleGame.UI.AddElement(_temp);
            }
            
            _currentColumn = 0;
        }
    }

    public override void Update(float dt)
    {
        base.Update(dt);

        if (_flash)
        {
            _timer += dt;

            for (int i = _currentColumn; i < _grid.Columns; i++)
                _grid.Slots[i, _currentRow].State = BirdleGrid.SlotState.Oops;

            if (_timer >= 1.0f)
            {
                _flash = false;
                _timer = 0;
                
                for (int i = _currentColumn; i < _grid.Columns; i++)
                    _grid.Slots[i, _currentRow].State = BirdleGrid.SlotState.None;
            }
        }
    }
}