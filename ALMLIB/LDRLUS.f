CD
CD*** SUBROUTINE LDRLUS ***********************************************
CD---------------------------------------------------------------------
CD    Назначение:
CD      решение системы линейных уравнений с вещественной LU-матрицей
CD---------------------------------------------------------------------
CD    Примечание:
CD       расположение массива матрицы по строкам для вызова из языка C
CD---------------------------------------------------------------------
CD    Вызов:
CD      CALL LDRLUS ( A,N,IA,B,IPVT )
CD---------------------------------------------------------------------
CD    Формальные параметры:
CD
CD      Имя  Размер Тип Тип_В/В   Назначение
CD
CD      A    IA*N     D     Вх     массив,содержащий LU-факторизованную
CD                                матрицу A;
CD      N       -     I     Вх     порядок матрицы A;
CD      IA      -     I     Вх     первая размерность массива A;
CD      B       N     D     В/В    массив, содержащий при вызове вектор
CD                                правой части, а при возврате - вектор
CD                                решения;
CD      IPVT    N     I     Вх     вектор, содержащий информацию о
CD                                выборе ведущих элементов:
CD                                 IPVT(K) - индекс K-й ведущей строки,
CD                                 IPVT(N) =(-1)**(число перестановок);
CD---------------------------------------------------------------------
CD    Требуемые подпрограммы и функции:
CD      стандартные: нет
CD---------------------------------------------------------------------
CD    Связь с внешними модулями:
CD      может использоваться самостоятельно или совместно с LDRLUF
CD--------------------------------------------------------------------
CD    Метод: 
CD      подпрограмма находит решение линейной системы уравнений A*X=B,
CD    где  A  -  LU-разложение матрицы A, X - вектор неизвестных, B -
CD    вектор правой части. Алгоритм включает в себя прямой и обратный
CD    ход.  
CD      Форсайт Дж., Малькольм М., Моулер К. Машинные методы 
CD    математических вычислений / М. изд.: Мир, 1980. - 280 c.
CD--------------------------------------------------------------------
CD    Внешняя среда: Электроника-79,   RSX-11M,   FORTRAN IV V02.2-1
CD--------------------------------------------------------------------
CD    Организация: МЭИ, кафедра РПДУ   Демидов В.М.         24.05.1989
CD                                     Медунов А.Л.         21.02.2015
CD--------------------------------------------------------------------
      SUBROUTINE LDRLUS(A, N, IA, B, IPVT)
C----------------------------------------------------------------------
C--   решение системы линейных уравнений с вещественной LU-матрицей
C----------------------------------------------------------------------
      IMPLICIT NONE
C--
      DOUBLE PRECISION A(IA,N)
      INTEGER          N
      INTEGER          IA
      DOUBLE PRECISION B(N)
      INTEGER          IPVT(N)
C--
      DOUBLE PRECISION T
      INTEGER          KB, KM1, NM1, NP1, I, K, M
C--
C--   аббревиатура имени модуля: LU-SOLUTION
C--
C--   случай N=1
      IF (N.EQ.1) GOTO 50
C--
C--   прямой ход
      NM1 = N - 1
      DO 20 K=1,NM1
         M = IPVT(K)
         T = B(M)
         B(M) = B(K)
         B(K) = T
         DO 10 I=K+1,N
            B(I) = B(I) + A(K,I)*T
   10    CONTINUE
   20 CONTINUE
C--
C--   обратный ход
      DO 40 KB=1,NM1
         KM1 = N - KB
         K = KM1 + 1
         B(K) = B(K) / A(K,K)
         T = -B(K)
         DO 30 I=1,KM1
            B(I) = B(I) + A(K,I)*T
   30    CONTINUE
   40 CONTINUE
C--
   50 B(1) = B(1) / A(1,1)
      RETURN
      END