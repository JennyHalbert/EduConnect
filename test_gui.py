import tkinter as tk
from tkinter import ttk, messagebox
import sys

print("========== duConnect GUI starting ==========")
# print("Using Python:", sys.executable)

# auth helpers 

def authenticate_user(role: str, username: str, password: str) -> bool:
    demo_accounts = {
        "student": {"1": "1"},
        "tutor": {"tutor1": "password456"},
    }
    return demo_accounts.get(role, {}).get(username) == password and bool(username)


def clear_entries(*entries: tk.Entry) -> None:
    for entry in entries:
        entry.delete(0, tk.END)


# student

def open_tutor_selection_window(parent: tk.Tk,
                                username: str,
                                priority: str,
                                subject: str,
                                selected_days: list[str]) -> None:
    """
    second screen
    shows table of tutors with rating + available days.
    user selects one row and clicks Submit.
    """
    top = tk.Toplevel(parent)
    top.title("Select Tutor")
    top.geometry("550x300")

    info = f"Student: {username}   |   Priority: {priority}   |   Subject: {subject}   |   Days: {', '.join(selected_days)}"
    ttk.Label(top, text=info, wraplength=520).pack(padx=10, pady=(10, 5))


    # sample data – was generated
    tutors = [
        {"name": "Tutor 1", "rating": 4.9, "days": "Tu"},
        {"name": "Tutor 2", "rating": 4.3, "days": "Th, Tu"},
        {"name": "Tutor 3", "rating": 4.7, "days": "M, W, F"},
    ]

    columns = ("name", "rating", "days")
    tree = ttk.Treeview(top, columns=columns, show="headings", selectmode="browse")
    tree.heading("name", text="Tutor")
    tree.heading("rating", text="Rating")
    tree.heading("days", text="Available Days")

    tree.column("name", width=180)
    tree.column("rating", width=80, anchor="center")
    tree.column("days", width=180)

    for t in tutors:
        tree.insert("", tk.END, values=(t["name"], t["rating"], t["days"]))

    tree.pack(expand=True, fill="both", padx=10, pady=5)

    def submit_request():
        selected = tree.selection()
        if not selected:
            messagebox.showwarning("No Tutor Selected", "Please select a tutor first.")
            return

        tutor_name, tutor_rating, tutor_days = tree.item(selected[0], "values")
        # Placeholder – this is where you'd call your matching / backend logic
        messagebox.showinfo(
            "Request Submitted",
            f"Request submitted!\n\n"
            f"Student: {username}\n"
            f"Tutor: {tutor_name} (Rating: {tutor_rating})\n"
            f"Subject: {subject}\n"
            f"Priority: {priority}\n"
            f"Session days: {', '.join(selected_days)}"
        )
        top.destroy()

    ttk.Button(top, text="Submit", command=submit_request).pack(pady=(0, 10))


def build_student_create_request_tab(parent: ttk.Notebook, username: str) -> ttk.Frame:
    tab = ttk.Frame(parent, padding=20)
    parent.add(tab, text="Create Request")

    ttk.Label(tab, text="Create Request", font=("Helvetica", 14, "bold")).grid(
        row=0, column=0, columnspan=4, pady=(0, 15)
    )

    # Priority (radio buttons)
    ttk.Label(tab, text="Priority (select one):").grid(row=1, column=0, sticky="w")
    priority_var = tk.StringVar(value="Medium")

    priorities = ["High", "Medium", "Low"]
    for i, p in enumerate(priorities):
        ttk.Radiobutton(
            tab, text=p, variable=priority_var, value=p
        ).grid(row=1, column=i + 1, padx=5, sticky="w")

    # Subject dropdown
    ttk.Label(tab, text="Select subject:").grid(row=2, column=0, sticky="w", pady=(15, 5))
    subject_var = tk.StringVar()
    subject_combo = ttk.Combobox(
        tab,
        textvariable=subject_var,
        values=["Math", "Physics", "Computer Science", "Chemistry", "Biology"],
        state="readonly",
        width=30,
    )
    subject_combo.grid(row=2, column=1, columnspan=3, sticky="w")
    subject_combo.current(0)

#     # Session day checkboxes
#     ttk.Label(tab, text="Session Day (select all that apply):").grid(
#         row=3, column=0, columnspan=4, sticky="w", pady=(20, 5)
#     )

# ## find here this happens 
#     days = ["Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"]
#     day_vars: list[tk.BooleanVar] = []
    
#     for i, d in enumerate(days):
#         var = tk.BooleanVar(value=False)
#         day_vars.append(var)
#         ttk.Checkbutton(tab, text=d, variable=var).grid(row=4, column=i, padx=3, sticky="w")

    # Session day checkboxes
    ttk.Label(tab, text="Session Day (select all that apply):").grid(
        row=3, column=0, columnspan=4, sticky="w", pady=(20, 5)
    )

    # Put days in their own frame so grid columns are independent
    days_frame = ttk.Frame(tab)
    days_frame.grid(row=4, column=0, columnspan=4, sticky="w")

    days = ["Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"]
    day_vars: list[tk.BooleanVar] = []

    for i, d in enumerate(days):
        var = tk.BooleanVar(value=False)
        day_vars.append(var)
        ttk.Checkbutton(days_frame, text=d, variable=var).grid(
            row=0, column=i, padx=3, sticky="w"
        )


    def on_next():
        chosen_days = [d for d, var in zip(days, day_vars) if var.get()]
        if not chosen_days:
            messagebox.showwarning("Missing Info", "Please select at least one session day.")
            return

        open_tutor_selection_window(
            tab.winfo_toplevel(),
            username=username,
            priority=priority_var.get(),
            subject=subject_var.get(),
            selected_days=chosen_days,
        )

    ttk.Button(tab, text="Next", command=on_next).grid(
        row=5, column=0, columnspan=4, pady=(20, 0), sticky="e"
    )

    # allow horizontal stretching
    for c in range(4):
        tab.columnconfigure(c, weight=1)

    return tab


def build_student_view_requests_tab(parent: ttk.Notebook, username: str) -> ttk.Frame:
    tab = ttk.Frame(parent, padding=20)
    parent.add(tab, text="View Requests")

    ttk.Label(tab, text=f"{username.title()}'s Requests", font=("Helvetica", 14, "bold")).pack(
        anchor="w", pady=(0, 10)
    )

    columns = ("tutor", "subject", "priority", "day", "status")
    tree = ttk.Treeview(tab, columns=columns, show="headings")
    for col in columns:
        tree.heading(col, text=col.title())
        tree.column(col, width=100, anchor="center")

    # Sample placeholder data
    sample_rows = [
        ("Tutor 1", "Math", "High", "Tu", "Pending"),
        ("Tutor 2", "CS", "Medium", "Th", "Accepted"),
    ]
    for row in sample_rows:
        tree.insert("", tk.END, values=row)

    tree.pack(expand=True, fill="both")

    ttk.Label(tab, text="(Demo data only – plug in your backend later.)").pack(
        anchor="w", pady=(5, 0)
    )

    return tab


def open_student_main_menu(root: tk.Tk, username: str) -> None:
    # Clear the login UI
    for widget in root.winfo_children():
        widget.destroy()

    root.title(f"EduConnect – Student Portal ({username})")
    root.geometry("650x400")
    root.resizable(True, True)

    notebook = ttk.Notebook(root)
    notebook.pack(expand=True, fill="both", padx=10, pady=10)

    build_student_create_request_tab(notebook, username)
    build_student_view_requests_tab(notebook, username)




def build_tutor_manage_info_tab(parent: ttk.Notebook, username: str) -> ttk.Frame:
    tab = ttk.Frame(parent, padding=20)
    parent.add(tab, text="Manage Info")

    ttk.Label(tab, text="Manage Tutor Info", font=("Helvetica", 14, "bold")).grid(
        row=0, column=0, columnspan=4, pady=(0, 10), sticky="w"
    )

    # Schedule checkboxes
    ttk.Label(tab, text="Schedule (select all that apply):").grid(
        row=1, column=0, columnspan=4, sticky="w", pady=(5, 5)
    )

    days = ["M", "T", "W", "Th", "F", "Sa", "Su"]
    day_vars: list[tk.BooleanVar] = []
    for i, d in enumerate(days):
        var = tk.BooleanVar(value=False)
        day_vars.append(var)
        ttk.Checkbutton(tab, text=d, variable=var).grid(row=2, column=i, padx=3, sticky="w")

    # Topic multi-select
    ttk.Label(tab, text="Select topics you can tutor:").grid(
        row=3, column=0, columnspan=4, sticky="w", pady=(20, 5)
    )

    topics = ["Math", "Physics", "Computer Science", "Chemistry", "Biology"]
    topics_listbox = tk.Listbox(tab, selectmode="multiple", height=5)
    for t in topics:
        topics_listbox.insert(tk.END, t)
    topics_listbox.grid(row=4, column=0, columnspan=4, sticky="nsew")

    def save_info():
        selected_days = [d for d, var in zip(days, day_vars) if var.get()]
        selected_topics = [
            topics[i] for i in topics_listbox.curselection()
        ]

        messagebox.showinfo(
            "Info Saved",
            f"Availability updated for {username}.\n\n"
            f"Days: {', '.join(selected_days) if selected_days else 'None'}\n"
            f"Topics: {', '.join(selected_topics) if selected_topics else 'None'}"
        )

    ttk.Button(tab, text="Save", command=save_info).grid(
        row=5, column=0, columnspan=4, pady=(10, 0), sticky="e"
    )

    # stretch
    for c in range(4):
        tab.columnconfigure(c, weight=1)
    tab.rowconfigure(4, weight=1)

    return tab


def build_tutor_view_requests_tab(parent: ttk.Notebook, username: str) -> ttk.Frame:
    tab = ttk.Frame(parent, padding=20)
    parent.add(tab, text="View Requests")

    ttk.Label(tab, text=f"{username.title()}'s Incoming Requests", font=("Helvetica", 14, "bold")).pack(
        anchor="w", pady=(0, 10)
    )

    columns = ("student", "subject", "priority", "day", "status")
    tree = ttk.Treeview(tab, columns=columns, show="headings")
    for col in columns:
        tree.heading(col, text=col.title())
        tree.column(col, width=100, anchor="center")

    # Sample placeholder data
    sample_rows = [
        ("student1", "Math", "High", "Tu", "Pending"),
        ("student2", "CS", "Low", "F", "Pending"),
    ]
    for row in sample_rows:
        tree.insert("", tk.END, values=row)

    tree.pack(expand=True, fill="both")

    ttk.Label(tab, text="(Demo data only – later you can accept/decline here.)").pack(
        anchor="w", pady=(5, 0)
    )

    return tab


def open_tutor_main_menu(root: tk.Tk, username: str) -> None:
    # Clear the login UI
    for widget in root.winfo_children():
        widget.destroy()

    root.title(f"EduConnect – Tutor Portal ({username})")
    root.geometry("650x400")
    root.resizable(True, True)

    notebook = ttk.Notebook(root)
    notebook.pack(expand=True, fill="both", padx=10, pady=10)

    build_tutor_manage_info_tab(notebook, username)
    build_tutor_view_requests_tab(notebook, username)




def handle_login(role: str, username_entry: tk.Entry, password_entry: tk.Entry) -> None:
    username = username_entry.get().strip()
    password = password_entry.get()
    root = username_entry.winfo_toplevel()

    if not username or not password:
        messagebox.showwarning("Please enter both username and password.")
        return

    if authenticate_user(role, username, password):
        clear_entries(username_entry, password_entry)
        print(f">>> Login successful for {username} as {role}")

        if role == "student":
            open_student_main_menu(root, username)
        else:
            open_tutor_main_menu(root, username)

    else:
        messagebox.showerror("Login Failed", "Invalid username or password.")
        password_entry.delete(0, tk.END)


def build_login_tab(parent: ttk.Notebook, role: str) -> None:
    print(f">>> Building tab for role: {role}")
    tab = ttk.Frame(parent, padding=20)
    parent.add(tab, text=f"{role.title()} Login")

    ttk.Label(
        tab,
        text=f"{role.title()} Portal",
        font=("Helvetica", 16, "bold")
    ).grid(row=0, column=0, columnspan=2, pady=(0, 20))

    ttk.Label(tab, text="Username:").grid(row=1, column=0, sticky="e", padx=(0, 10), pady=5)
    username_entry = ttk.Entry(tab, width=30)
    username_entry.grid(row=1, column=1, sticky="w", pady=5)

    ttk.Label(tab, text="Password:").grid(row=2, column=0, sticky="e", padx=(0, 10), pady=5)
    password_entry = ttk.Entry(tab, width=30, show="*")
    password_entry.grid(row=2, column=1, sticky="w", pady=5)

    login_button = ttk.Button(
        tab,
        text="Submit",
        command=lambda: handle_login(role, username_entry, password_entry),
    )
    login_button.grid(row=3, column=0, columnspan=2, pady=(15, 5), sticky="we")

    forgot_pw = ttk.Button(
        tab,
        text="Forgot password?",
        command=lambda: messagebox.showinfo(
            "Unknown Password",
            "Contact help@educonnect.com to reset your password."
        )
    )
    forgot_pw.grid(row=4, column=0, columnspan=2, pady=(0, 5), sticky="we")

    tab.columnconfigure(0, weight=0)
    tab.columnconfigure(1, weight=1)


def main() -> None:
    root = tk.Tk()
    root.title("EduConnect Login")
    root.geometry("450x320")
    root.resizable(False, False)

    notebook = ttk.Notebook(root)
    notebook.pack(expand=True, fill="both", padx=20, pady=20)

    build_login_tab(notebook, "student")
    build_login_tab(notebook, "tutor")

    root.mainloop()


if __name__ == "__main__":
    main()



