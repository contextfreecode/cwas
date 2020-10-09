using System;
using System.Text;

class Field {
    public string Label { init; get; }

    public int Max { init; get; }

    StringBuilder value = new();

    void Draw() {
        Console.Write($"\r{new string(' ', Console.WindowWidth)}\r");
        Console.ForegroundColor = ConsoleColor.Green;
        Console.Write($"{Label}: ");
        Console.ResetColor();
        Console.Write(value);
    }

    bool HandleChar() {
        var c = Console.ReadKey(true);
        switch (c.KeyChar) {
            case '\r':
                return false;
            case '\x7F':
                if (value.Length > 0) {
                    value.Length -= 1;
                }
                break;
            default:
                if (value.Length < Max) {
                    value.Append(c.KeyChar);
                }
                break;
        }
        return true;
    }

    public string Prompt() {
        Draw();
        while (HandleChar()) {
            Draw();
        }
        Console.WriteLine();
        return value.ToString();
    }
}

class Program {
    static void Main(string[] args) {
        var nameField = new Field { Label = "Name", Max = 5 };
        nameField.Prompt();
    }
}
