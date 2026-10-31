# wordbank processor
# makes everything uppercase, sorts it alphabetically,
# and flags + removes naughty bad nono words

import sys

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Please provide a wordbank file!")
        sys.exit(2)

    filename = sys.argv[1]
    lines = []
    words = []

    with open(filename, "r") as f:
        for line in f:
            line = line.strip()
            if line != '' and not line.startswith('#'):
                words.append(line.upper())
            else:
                lines.append(line) # preserve all other lines in the file

    words.sort()
    BANNED = ["FUCK", "SHIT", "CUNT", "ARSES", "HORNY", "BITCH"]

    with open(filename, "w") as f:
        for line in lines:
            f.write(f"{line}\n")

        for word in words:
            #for ban in BANNED:
            #    if ban in word:
            #        print(f"Blocked word: {word}")
            #        continue
            f.write(f"{word}\n")
