# RC1 — Control de producción de componentes electrónicos

**Materia:** Programación 1 (ISWZ1102)  
**Lenguaje:** C11  
**Integrante:** Nicolás Tituaña

Este proyecto calcula los minutos y recursos que necesita una fábrica para cubrir la demanda de hasta cinco productos. Permite registrar, listar, buscar y editar por nombre, eliminar y configurar las capacidades disponibles. No usa `struct`, archivos para persistir datos, ni funciones predefinidas de ordenamiento o búsqueda.

## 1. Ejecutar en Windows (MinGW/GCC)

Abre PowerShell o la terminal de VS Code dentro de la carpeta que contiene `main.c` y ejecuta:

```powershell
gcc -std=c11 -Wall -Wextra -Wpedantic main.c -o produccion.exe
.\produccion.exe
```

Si el sistema no reconoce `gcc`, instala o configura un compilador C compatible con GCC y abre una terminal nueva.

## 2. Ejecutar en Linux / macOS

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic main.c -o produccion
./produccion
```

## 3. Menú del programa

- **1:** registrar un producto (máximo cinco nombres únicos).
- **2:** listar productos y sus valores unitarios.
- **3:** buscar por nombre y editar el nombre, la demanda, los minutos por unidad o los recursos por unidad.
- **4:** buscar por nombre y eliminar el producto.
- **5:** configurar los minutos y recursos disponibles.
- **6:** mostrar los requerimientos totales, déficits y factibilidad.
- **0:** salir.

El programa solicita cantidades enteras. No guarda los registros en archivos; al cerrarlo se pierde la sesión, en cumplimiento de la consigna.

## 4. Ejemplo para probar

| Producto | Demanda | Minutos/unidad | Recursos/unidad |
|---|---:|---:|---:|
| Resistencias | 10 | 4 | 2 |
| Capacitores | 8 | 7 | 4 |
| Sensores | 6 | 5 | 3 |
| Chips | 4 | 9 | 5 |
| Conectores | 12 | 3 | 1 |

En la opción 5 ingresa **220 minutos y 110 recursos**. En la opción 6, el resultado debe ser **198 minutos**, **102 recursos** y **DEMANDA FACTIBLE**.

## 5. Ejecutar pruebas automatizadas

Después de compilar el programa, si tienes Python 3 ejecuta:

```bash
python verificar_pruebas.py
```

El script verifica **ocho escenarios**, entre ellos insuficiencia de recursos, insuficiencia de tiempo, cambios de datos, eliminación, límites de capacidad y entrada inválida. Las salidas de la consola están en `prueba_*.txt`.

## 6. Diagrama de flujo y evidencias

- Abre `diagrama_flujo_RC1.drawio` en https://app.diagrams.net para ver sus **dos páginas editables**: menú general y evaluación de factibilidad.
- `diagrama_menu.png` y `diagrama_evaluacion.png` son versiones visibles de los diagramas.
- La carpeta `evidencias/` contiene capturas de fragmentos del código y salidas de las pruebas.
- El informe en Word incluye descripción, objetivos, tabla de variables, comparación de alternativas, justificación, diagramas, capturas, resultados, referencias en español APA 7 y el código fuente completo.

## 7. Paso pendiente para la entrega: enlace real de GitHub

La consigna **exige un enlace de GitHub**. No hay una dirección válida hasta que el proyecto se publique desde una cuenta con autorización. Crea un repositorio, sube `main.c`, `README.md` y el `.drawio`, copia la URL real y reemplaza el texto **PENDIENTE DE PUBLICACIÓN** en la sección 3.7 del informe.

No coloques como dirección real `https://github.com/USUARIO/NOMBRE-DEL-REPOSITORIO`, pues es solo un ejemplo de formato.
