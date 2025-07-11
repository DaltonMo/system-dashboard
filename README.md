# 🧠 System Telemetry Dashboard

A lightweight system monitoring tool with a C backend that collects real-time telemetry data and a React + TypeScript frontend that visualizes system metrics in a responsive web dashboard.

---

## 🔍 Why This Project Matters

This project showcases full-stack development with systems-level programming and real-time data visualization. It emphasizes:

- Reading and parsing Linux system files (e.g., `/proc/meminfo`)
- JSON serialization from C
- Backend-to-frontend data flow
- Real-time dashboard UI with modern React

---

## 🛠️ Tech Stack

- **Language:** C, TypeScript
- **Tools:** GCC, React, Node.js, VSCode
- **Concepts:** System monitoring, file I/O, JSON formatting, UI rendering, periodic data refresh

---

## 🚀 How to Run

### Backend (Telemetry Writer)

```bash
cd system-dashboard/backend/telemetry
gcc -o telemetry telemetry.c
./telemetry
```

### Frontend (React Dashboard)

```bash
cd system-dashboard/frontend
npm install
npm run dev
```

> 🔧 Ensure `stats.json` is periodically updated by the backend for live dashboard data. This is currently written to `../data/stats.json`.

---

## 📘 What I Learned

- How to read and parse system-level files like `/proc/meminfo`
- Writing structured JSON from a C program
- Linking backend-generated telemetry data to a frontend interface
- Using statvfs and other APIs to get disk and uptime metrics
- Building a responsive React dashboard with TypeScript

---

## 📁 File Structure

```
system-dashboard/
├── backend/
│   └── telemetry/
│       └── telemetry.c
├── data/
│   ├── data.json
│   └── stats.json
├── frontend/
└── README.md
```

---

## 🧑‍💻 Author

**Dalton Mosley**
[GitHub](https://github.com/DaltonMo) • [LinkedIn](https://www.linkedin.com/in/dalton-lee-mosley/)
