document.getElementById('button').addEventListener('click', function (event) {
    event.preventDefault(); // Предотвращаем отправку формы

    const functionInputValue = document.getElementById('functionInput').value;

    const data = {
        function: functionInputValue // Функция для отправки на сервер
    };

    fetch('http://localhost:8080', {
        method: 'POST',
        headers: {
            "Content-Type": "application/json"
        },
        body: JSON.stringify(data)
    })
        .then(response => {
            if (!response.ok) {
                throw new Error('Network response was not ok');
            }
            return response.json(); // Парсим JSON-ответ
        })
        .then(data => {
            console.log('Ответ от сервера:', data);  // Выводим данные ответа в консоль

            if (data && Array.isArray(data.coordinates) && Array.isArray(data.breakPoints)) {
                plotGraph(data); // Построение графика
                displaySteps(data); // Отображение шагов
            } else {
                throw new Error('Invalid data format: coordinates or breakPoints are missing or malformed');
            }
        })
        .catch(error => {
            console.error('Error:', error);
            alert('Произошла ошибка: ' + error.message); // Выводим сообщение об ошибке
        });
});

function plotGraph(data) {
    const { coordinates, breakPoints } = data;

    // Логика вставки разрывов перемещена сюда
    console.log("Initial Coordinates:", coordinates);
    console.log("Break Points:", breakPoints);

    const modifiedCoordinates = [];
    const xValues = [];
    const yValues = [];
    let breakIndex = 0;

    coordinates.forEach(coord => {
        // Добавляем текущую координату
        modifiedCoordinates.push(coord);
        if (coord[0] !== undefined && coord[1] !== undefined) {
            xValues.push(coord[1]);
            yValues.push(coord[0]);
        } else {
            console.warn('Invalid coordinate:', coord);
        }

    // Проверяем разрывные точки после добавления основной координаты
    while (breakIndex < breakPoints.length && breakPoints[breakIndex][0] <= coord[1] + 0.01) {
        const breakPointX = breakPoints[breakIndex][0]; // x координата разрыва
        if (Math.abs(breakPointX - coord[1]) <= 0.01) {
            console.log(`Adding break point after coord ${coord}: [null, ${breakPointX}]`);
            modifiedCoordinates.push([null, breakPointX]); // Вставляем разрыв после координаты
            xValues.push(breakPointX); // Добавляем x разрыва
            yValues.push(null); // Добавляем y разрыва
        }
        breakIndex++;
    }
    });

    // Добавляем отладочную информацию
    console.log("Modified Coordinates:", modifiedCoordinates);

    const trace = {
        x: xValues,
        y: yValues,
        type: 'scatter'
    };

    const layout = {
        title: data.function, // Название функции
        xaxis: { title: 'Y' },
        yaxis: { title: 'X' }
    };

    const config = {
        responsive: false, // График адаптируется под размер контейнера
        displayModeBar: true, // Панель инструментов
        scrollZoom: true // Возможность масштабирования с помощью скролла
    };

    const plotData = [trace];

    Plotly.newPlot('plot', plotData, layout, config); // Построение графика с Plotly
    document.getElementById('plot').style.display = 'block'; // Показываем график
}

function displaySteps(data) {
    const stepsList = document.getElementById('stepsList');
    stepsList.innerHTML = ''; // Очищаем предыдущие шаги

    // Шаг 1: Область определения
    const definitionItem = document.createElement('li');
    if (data.domain && Array.isArray(data.domain)) {
        const formattedDomain = data.domain.map(range => {
            let start = range[0];
            let end = range[1];

            // Проверка и замена границ на символ бесконечности
            if (start === 10000000000) start = '∞';
            if (start === -10000000000) start = '-∞';
            if (end === 10000000000) end = '∞';
            if (end === -10000000000) end = '-∞';

            return `[${start}, ${end}]`;
        }).join(', ');

        definitionItem.textContent = `Область определения: ${formattedDomain}`;
    } else {
        definitionItem.textContent = 'Область определения: Не определена';
    }
    stepsList.appendChild(definitionItem);

    // Шаг 2: Точки разрыва
    const breakPointsItem = document.createElement('li');
    if (data.breakPoints && data.breakPoints.length > 0) {
        const breakPointsText = data.breakPoints.map(point => {
            let x = point[0];
            let type = point[1];
            let description = '';

            // Определение типа точки разрыва
            if (type === 1) {
                description = 'Устранимая точка разрыва 1 рода';
            } else if (type === 2) {
                description = 'Неустранимая точка разрыва 1 рода';
            } else if (type === 3) {
                description = 'Точка разрыва 2 рода';
            }

            return `x = ${x} (${description})`;
        }).join(', ');

        breakPointsItem.textContent = `Точки разрыва: ${breakPointsText}`;
    } else {
        breakPointsItem.textContent = 'Точки разрыва: Нет';
    }
    stepsList.appendChild(breakPointsItem);

    // Шаг 3: Вертикальные асимптоты
    const verticalAsymptotesItem = document.createElement('li');
    if (data.asymptotes && data.asymptotes.vertical_asymptotes && data.asymptotes.vertical_asymptotes.length > 0) {
        verticalAsymptotesItem.textContent = `Вертикальные асимптоты: x = ${data.asymptotes.vertical_asymptotes.join(', ')}`;
    } else {
        verticalAsymptotesItem.textContent = 'Вертикальные асимптоты: Нет';
    }
    stepsList.appendChild(verticalAsymptotesItem);

    // Шаг 4: Горизонтальная асимптота
    const horizontalAsymptoteItem = document.createElement('li');
    if (data.asymptotes && data.asymptotes.horizontal_asymptote[0] != 10000000000 && data.asymptotes.horizontal_asymptote[1] != 10000000000 && data.asymptotes.vertical_asymptotes) {
        horizontalAsymptoteItem.textContent = `Горизонтальная асимптота: y = ${data.asymptotes.horizontal_asymptote[1]}`;
    } else {
        horizontalAsymptoteItem.textContent = 'Горизонтальная асимптота: Нет';
    }
    stepsList.appendChild(horizontalAsymptoteItem);

    // Шаг 5: Симметрия
    const symmetryItem = document.createElement('li');
    if (data.symmetry) {
        symmetryItem.textContent = `Симметрия: ${data.symmetry}`;
    } else {
        symmetryItem.textContent = 'Симметрия: Нет информации';
    }
    stepsList.appendChild(symmetryItem);

    // Шаг 6: Периодичность
    const periodicityItem = document.createElement('li');
    if (data.periodicity) {
        periodicityItem.textContent = `Периодичность: ${data.periodicity}`;
    } else {
        periodicityItem.textContent = 'Периодичность: Нет информации';
    }
    stepsList.appendChild(periodicityItem);

    // Шаг 7: Интервалы монотонности
    const monotonyIntervalsItem = document.createElement('li');
    if (data.monotonyIntervals && data.monotonyIntervals.length > 0) {
        const monotonyIntervalsText = data.monotonyIntervals.map(interval => {
            let start = interval[0];
            let end = interval[1];
            let type = interval[2];
            let description = '';

            // Проверка и замена границ на символ бесконечности
            if (start === 10000000000) start = '∞';
            if (start === -10000000000) start = '-∞';
            if (end === 10000000000) end = '∞';
            if (end === -10000000000) end = '-∞';

            // Определение типа интервала
            if (type === 1) {
                description = 'возрастает';
            } else if (type === -1) {
                description = 'убывает';
            }

            return `[${start}, ${end}] - (${description})`;
        }).join(', ');

        monotonyIntervalsItem.textContent = `Интервалы монотонности: ${monotonyIntervalsText}`;
    } else {
        monotonyIntervalsItem.textContent = 'Интервалы монотонности: Нет';
    }
    stepsList.appendChild(monotonyIntervalsItem);

    // Шаг 8: Интервалы выпуклости/вогнутости
    const convexityConcavityIntervalsItem = document.createElement('li');
    if (data.convexityConcavityIntervals && data.convexityConcavityIntervals.length > 0) {
        const convexityConcavityIntervalsText = data.convexityConcavityIntervals.map(interval => {
            let start = interval[0];
            let end = interval[1];
            let type = interval[2];
            let description = '';

            // Проверка и замена границ на символ бесконечности
            if (start === 10000000000) start = '∞';
            if (start === -10000000000) start = '-∞';
            if (end === 10000000000) end = '∞';
            if (end === -10000000000) end = '-∞';

            // Определение типа интервала
            if (type === -1) {
                description = 'Выпуклая';
            } else if (type === 1) {
                description = 'Вогнутая';
            }

            return `[${start}, ${end}] - (${description})`;
        }).join(', ');

        convexityConcavityIntervalsItem.textContent = `Интервалы выпуклости/вогнутости: ${convexityConcavityIntervalsText}`;
    } else {
        convexityConcavityIntervalsItem.textContent = 'Интервалы выпуклости/вогнутости: Нет';
    }
    stepsList.appendChild(convexityConcavityIntervalsItem);

    // Показываем раздел с шагами
    document.getElementById('steps').style.display = 'block';
}