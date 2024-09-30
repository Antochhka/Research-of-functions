function plotGraph(event) {
    event.preventDefault(); // Предотвращаем отправку формы

    const input = document.getElementById('functionInput').value;
    const xValues = [];
    const yValues = [];

    // Генерация значений x и y
    for (let x = -10; x <= 10; x += 0.1) {
        xValues.push(x);
        try {
            // Вычисление значения функции
            yValues.push(eval(input));
        } catch (error) {
            alert('Ошибка в функции. Проверьте правильность написания.');
            return;
        }
    }

    const trace = {
        x: xValues,
        y: yValues,
        type: 'scatter'
    };

    const data = [trace];

    Plotly.newPlot('plot', data);
}

document.querySelector('button').addEventListener('click', function (e) {
    var div = document.querySelector('div')
    div.style.display = div.style.display = 'block'
})