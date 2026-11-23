
const defaultNotices = [
    {
        id: 1,
        title: "Welcome to NoticeFlow",
        description: "Our new community noticeboard is now available.",
        category: "General",
        priority: "Normal"
    },
    {
        id: 2,
        title: "Annual College Fest",
        description: "Registrations are open for the upcoming college fest.",
        category: "Events",
        priority: "Urgent"
    },
    {
        id: 3,
        title: "Internal Examination Update",
        description: "Check your department timetable for upcoming examinations.",
        category: "Education",
        priority: "Normal"
    }
];

const STORAGE_KEY = "noticeflow_notices_v2";

function loadNotices() {
    try {
        const saved = localStorage.getItem(STORAGE_KEY);

        if (saved !== null) {
            const parsed = JSON.parse(saved);
            if (Array.isArray(parsed)) return parsed;
        }
    } catch (error) {
        console.error("Loading error:", error);
    }

    return defaultNotices.map(notice => ({ ...notice }));
}

let notices = loadNotices();
let currentCategory = "All";

function saveNotices() {
    try {
        localStorage.setItem(
            STORAGE_KEY,
            JSON.stringify(notices)
        );
        return true;
    } catch (error) {
        console.error("Saving error:", error);
        alert("Could not save notices.");
        return false;
    }
}

function escapeHTML(value) {
    return String(value).replace(/[&<>"']/g, char => ({
        "&": "&amp;",
        "<": "&lt;",
        ">": "&gt;",
        '"': "&quot;",
        "'": "&#39;"
    })[char]);
}

function displayNotices() {
    const list = document.getElementById("noticeList");
    const search = document.getElementById("searchInput")
        .value.trim().toLowerCase();

    let filtered = notices.filter(notice => {
        const categoryMatch =
            currentCategory === "All" ||
            notice.category === currentCategory;

        const searchMatch =
            notice.title.toLowerCase().includes(search) ||
            notice.description.toLowerCase().includes(search);

        return categoryMatch && searchMatch;
    });

    filtered.sort((a, b) =>
        (b.priority === "Urgent") - (a.priority === "Urgent")
    );

    list.innerHTML = "";

    if (filtered.length === 0) {
        list.innerHTML = "<p>No notices found.</p>";
    }

    filtered.forEach(notice => {
        const card = document.createElement("article");
        card.className = "notice-card";

        card.innerHTML = `
            <span class="category">
                ${escapeHTML(notice.category)}
            </span>
            <span class="${notice.priority === "Urgent" ? "urgent" : "normal"}">
                ${escapeHTML(notice.priority)}
            </span>
            <h3>${escapeHTML(notice.title)}</h3>
            <p>${escapeHTML(notice.description)}</p>
        `;

        list.appendChild(card);
    });

    document.getElementById("noticeCount").textContent =
        filtered.length + " notices";
}

function filterNotices(category) {
    currentCategory = category;

    document.querySelectorAll(".filter").forEach(button => {
        button.classList.toggle(
            "active",
            button.textContent.trim() === category
        );
    });

    displayNotices();
}

function showAdmin() {
    document.getElementById("adminPanel").classList.remove("hidden");
}

function hideAdmin() {
    document.getElementById("adminPanel").classList.add("hidden");
}

function addNotice() {
    const title = document.getElementById("noticeTitle").value.trim();
    const description =
        document.getElementById("noticeDescription").value.trim();

    if (!title || !description) {
        alert("Please enter title and description.");
        return;
    }

    const newNotice = {
        id: Date.now(),
        title,
        description,
        category: document.getElementById("noticeCategory").value,
        priority: document.getElementById("noticePriority").value
    };

    notices.push(newNotice);

    if (!saveNotices()) {
        notices.pop();
        return;
    }

    document.getElementById("noticeTitle").value = "";
    document.getElementById("noticeDescription").value = "";

    currentCategory = "All";

    document.querySelectorAll(".filter").forEach(button => {
        button.classList.toggle(
            "active",
            button.textContent.trim() === "All"
        );
    });

    hideAdmin();
    displayNotices();

    alert("Notice published successfully!");
}

function deleteNotice(id) {
    notices = notices.filter(notice => notice.id !== id);

    if (saveNotices()) {
        displayNotices();
    }
}

document.getElementById("searchInput")
    .addEventListener("input", displayNotices);

document.getElementById("themeBtn").addEventListener("click", function () {
    document.body.classList.toggle("dark");

    const dark = document.body.classList.contains("dark");

    this.textContent = dark ? "☀️ Light Mode" : "🌙 Dark Mode";

    localStorage.setItem("noticeflow_theme", dark ? "dark" : "light");
});

if (localStorage.getItem("noticeflow_theme") === "dark") {
    document.body.classList.add("dark");
    document.getElementById("themeBtn").textContent = "☀️ Light Mode";
}

// Save default notices on first launch
if (localStorage.getItem(STORAGE_KEY) === null) {
    saveNotices();
}

// Make functions available to HTML buttons
window.filterNotices = filterNotices;
window.showAdmin = showAdmin;
window.hideAdmin = hideAdmin;
window.addNotice = addNotice;
window.deleteNotice = deleteNotice;

// Start the application
displayNotices();
