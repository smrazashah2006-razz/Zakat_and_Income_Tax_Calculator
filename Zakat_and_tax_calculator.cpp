/*
 * Zakat and Income Tax Calculator (Pakistan) - GUI version
 * Uses the Windows API (windows.h). No extra library to install.
 * Works in Dev-C++ (Windows Application project).
 */

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NISAB       503529.0
#define ZAKAT_RATE  0.025

/* IDs for the buttons */
#define ID_BTN_ZAKAT  1
#define ID_BTN_TAX    2
#define ID_BTN_BOTH   3

/* Handles of the text boxes */
HWND hIncome;
HWND hResult;


/* Function 1: calculate Zakat */
double calculate_Zakat(double income)
{
    double zakat = 0;

    if (income >= NISAB)
    {
        zakat = income * ZAKAT_RATE;
    }

    return zakat;
}


/* Function 2: calculate Income Tax (salaried slabs, Tax Year 2027) */
double Calculate_Incometax(double income)
{
    double tax;

    if (income <= 600000)
        tax = 0;
    else if (income <= 1200000)
        tax = (income - 600000) * 0.01;
    else if (income <= 2200000)
        tax = 6000 + (income - 1200000) * 0.11;
    else if (income <= 3200000)
        tax = 116000 + (income - 2200000) * 0.20;
    else if (income <= 4100000)
        tax = 316000 + (income - 3200000) * 0.25;
    else if (income <= 5600000)
        tax = 541000 + (income - 4100000) * 0.29;
    else if (income <= 7000000)
        tax = 976000 + (income - 5600000) * 0.32;
    else
        tax = 1424000 + (income - 7000000) * 0.35;

    return tax;
}


/* Reads the income, calculates, and shows the answer in the result box */
/* choice: 1 = Zakat, 2 = Income Tax, 3 = Both                           */
void ShowResult(int choice)
{
    char text[50];
    char result[500];
    char line[150];
    double income;
    double zakat;
    double tax;

    GetWindowTextA(hIncome, text, 50);
    income = atof(text);

    if (income <= 0)
    {
        MessageBoxA(NULL, "Please enter an income greater than 0.",
                    "Invalid Input", MB_OK | MB_ICONWARNING);
        return;
    }

    result[0] = '\0';

    sprintf(line, "Total Income: Rs %.2f\r\n\r\n", income);
    strcat(result, line);

    if (choice == 1 || choice == 3)
    {
        zakat = calculate_Zakat(income);

        if (zakat == 0)
            sprintf(line, "Zakat: Not applicable (below Nisab of Rs %.0f)\r\n", NISAB);
        else
            sprintf(line, "Zakat Payable: Rs %.2f\r\n", zakat);

        strcat(result, line);
    }

    if (choice == 2 || choice == 3)
    {
        tax = Calculate_Incometax(income);

        sprintf(line, "Income Tax: Rs %.2f\r\n", tax);
        strcat(result, line);

        sprintf(line, "Income after tax: Rs %.2f\r\n", income - tax);
        strcat(result, line);
    }

    SetWindowTextA(hResult, result);
}


/* Handles everything that happens in the window */
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_CREATE:
            /* Label */
            CreateWindowA("STATIC", "Enter your total ANNUAL income (Rs):",WS_VISIBLE | WS_CHILD,20, 20, 320, 20, hwnd, NULL, NULL, NULL);

            /* Income text box */
            hIncome = CreateWindowA("EDIT", "",WS_VISIBLE | WS_CHILD | WS_BORDER,20, 45, 320, 25, hwnd, NULL, NULL, NULL);

            /* Three buttons */
            CreateWindowA("BUTTON", "Zakat",WS_VISIBLE | WS_CHILD,20, 85, 100, 30, hwnd, (HMENU)ID_BTN_ZAKAT, NULL, NULL);

            CreateWindowA("BUTTON", "Income Tax",WS_VISIBLE | WS_CHILD,130, 85, 100, 30, hwnd, (HMENU)ID_BTN_TAX, NULL, NULL);

            CreateWindowA("BUTTON", "Both",WS_VISIBLE | WS_CHILD,240, 85, 100, 30, hwnd, (HMENU)ID_BTN_BOTH, NULL, NULL);

            /* Result box (read only) */
            hResult = CreateWindowA("EDIT", "",WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_READONLY,20, 135, 320, 120, hwnd, NULL, NULL, NULL);
            break;

        case WM_COMMAND:
            if (LOWORD(wParam) == ID_BTN_ZAKAT)
                ShowResult(1);
            else if (LOWORD(wParam) == ID_BTN_TAX)
                ShowResult(2);
            else if (LOWORD(wParam) == ID_BTN_BOTH)
                ShowResult(3);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProcA(hwnd, msg, wParam, lParam);
    }

    return 0;
}


/* Program starts here (instead of main) */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrev, LPSTR args, int nShow)
{
    WNDCLASSA wc;
    HWND hwnd;
    MSG msg;

    memset(&wc, 0, sizeof(wc));
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = hInstance;
    wc.hCursor       = LoadCursorA(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
    wc.lpszClassName = "ZakatTaxWindow";

    RegisterClassA(&wc);

    hwnd = CreateWindowA("ZakatTaxWindow", "Zakat and Income Tax Calculator",WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE,200, 150, 375, 320, NULL, NULL, hInstance, NULL);

    /* Keep the window running until it is closed */
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
