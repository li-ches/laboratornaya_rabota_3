package main

import (
	"fmt"
	"math"
)

func calculateFunction(x float64) float64 {
	if x <= -8.0 {
		return -3.0
	} else if x > -8.0 && x <= -3.0 {
		return (3.0/5.0)*x + (9.0/5.0)
	} else if x > -3.0 && x <= 3.0 {
		return -math.Sqrt(9.0 - x*x)
	} else if x > 3.0 && x <= 5.0 {
		return x - 3.0
	} else {
		return 3.0
	}
}

func main() {
	const xStart = -10.0 // Начало интервала X
	const xEnd = 8.0     // Конец интервала X
	const dx = 1.0       // Шаг изменения X

	// Вывод "шапки" таблицы
	fmt.Println("+-----------------------+")
	fmt.Println("|    x    |    f(x)   |")
	fmt.Println("+-----------------------+")

	// Циклический вычислительный процесс
	for x := xStart; x <= xEnd; x += dx {
		y := calculateFunction(x)

		// Вывод строки таблицы с результатами
		fmt.Printf("| %7.4f | %9.4f |\n", x, y)
	}

	// Вывод "подвала" таблицы
	fmt.Println("+-----------------------+")
}
