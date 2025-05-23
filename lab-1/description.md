# Лабораторная работа 1

## [# 25] Квадратичная задача о назначениях.

Есть множество $n$ предприятий, которые могут быть расположены в $n$ местах. Для каждой пары мест задано расстояние и для каждой пары производств задано количество материала, перевозимого между двумя производствами. Требуется расставить производства по местам (два производства нельзя размещать в одном месте) таким образом, что сумма расстояний, умноженных на соответствующие потоки, будет минимальной. Решить задачу для $N = 10$, $N = 15$, где $N$ – количество вершин в графе. Оценить время работы программы для входа $N = 20$ и $N = 50$.

$n$ – число объектов и мест их назначения;

$c_{ij}$ – затраты на передачу единицы потока ресурсов из пункта $i$ в пункт $j$;

$q_{kp}$ – объём ресурсов, направляемых от объекта $k$ к объекту $p$;

Элементы $c_{ij}$ и $q_{kp}$ задаются в виде соответствующих матриц $C$ и $Q$ размерности $n \times n$.

$R$ – общие затраты, необходимые для обмена ресурсами между всеми объектами.

$$ R \rightarrow \min $$

# Замер времени

В комбинаторных лабораторных работах требуется выводить время работы программы в зависимости от количества входных данных. Укажите время работы в секундах для $n = 5$; $n = 10$; $n = 20$; $n = 50$; $n = 100$. Для больших данных напишите функцию выполняющую замер времени. Сравните с возрастом Земли – 4.54 миллиарда лет, с возрастом Вселенной – 13.8 миллиардов лет.