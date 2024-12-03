document.addEventListener("DOMContentLoaded", () => {
    const cells = document.querySelectorAll(".cell");
    let currentPlayer = "X";
    const board = ["", "", "", "", "", "", "", "", ""];
    const winningCombinations = [
        [0, 1, 2],
        [3, 4, 5],
        [6, 7, 8],
        [0, 3, 6],
        [1, 4, 7],
        [2, 5, 8],
        [0, 4, 8],
        [2, 4, 6]
    ];

    cells.forEach(cell => {
        cell.addEventListener("click", handleClick);
    });

    function handleClick(event) {
        const index = event.target.dataset.index;

        if (board[index] === "") {
            board[index] = currentPlayer;
            event.target.textContent = currentPlayer;
            if (checkWin()) {
                setTimeout(() => {
                    alert(`${currentPlayer}が勝ちました！`);
                    resetGame();
                }, 100);
            } else if (board.every(cell => cell !== "")) {
                setTimeout(() => {
                    alert("引き分けです！");
                    resetGame();
                }, 100);
            } else {
                currentPlayer = currentPlayer === "X" ? "O" : "X";
            }
        }
    }

    function checkWin() {
        return winningCombinations.some(combination => {
            return combination.every(index => {
                return board[index] === currentPlayer;
            });
        });
    }

    function resetGame() {
        board.fill("");
        cells.forEach(cell => {
            cell.textContent = "";
        });
        currentPlayer = "X";
    }
});
