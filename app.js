const SmartCityUI = {
    storageKey: 'smartcity-vehicles',
    data: {
        vehicles: [],
        history: [],
        incomingQueue: [],
        tollQueue: [],
        emergencyQueue: [],
        graph: {
            nodes: 8,
            edges: [
                [0, 1],
                [0, 2],
                [1, 3],
                [2, 4],
                [3, 5],
                [4, 6],
                [5, 7],
                [6, 7]
            ]
        }
    },
    init() {
        this.loadData();
        this.bindPage();
        this.renderDashboard();
    },
    loadData() {
        const saved = localStorage.getItem(this.storageKey);
        if (saved) {
            this.data = JSON.parse(saved);
            this.data.vehicles = this.data.vehicles.map(v => ({
                parkingFee: v.parkingFee != null ? v.parkingFee : (v.fee != null ? v.fee : 0),
                tollFee: v.tollFee != null ? v.tollFee : 0,
                ...v
            }));
            this.data.history = this.data.history || [];
            this.data.incomingQueue = this.data.incomingQueue || [];
            this.data.tollQueue = this.data.tollQueue || [];
            this.data.emergencyQueue = this.data.emergencyQueue || [];
        } else {
            this.data.vehicles = [
                { number: 'ABC1234', owner: 'Rohit Kumar', type: 'Car', emergency: false, slot: 2, parkingFee: 50, tollFee: 0 },
                { number: 'DQT3120', owner: 'Nisha Sharma', type: 'Truck', emergency: false, slot: 5, parkingFee: 65, tollFee: 0 }
            ];
            this.data.history = [];
            this.data.incomingQueue = [];
            this.data.tollQueue = [];
            this.data.emergencyQueue = [];
            this.saveData();
        }
    },
    saveData() {
        localStorage.setItem(this.storageKey, JSON.stringify(this.data));
    },
    getAvailableSlot() {
        const used = this.data.vehicles.map(v => v.slot);
        for (let i = 1; i <= 50; i++) {
            if (!used.includes(i)) return i;
        }
        return null;
    },
    calculateFee(hours = 1) {
        return 35 + 15 * hours;
    },
    calculateTollFee(type) {
        if (type === 'Car') return 50;
        if (type === 'Bike') return 20;
        if (type === 'Truck') return 100;
        return 0;
    },
    forwardIncomingToToll(number) {
        const idx = this.data.incomingQueue.findIndex(v => v.number === number);
        if (idx === -1) return false;
        const vehicle = this.data.incomingQueue.splice(idx, 1)[0];
        const enqueued = this.enqueueToll(vehicle);
        if (!enqueued) {
            this.data.incomingQueue.splice(idx, 0, vehicle);
            this.saveData();
        }
        return enqueued;
    },
    parkVehicle(vehicle) {
        const slot = this.getAvailableSlot();
        if (!slot) return { success: false, message: 'Parking is full.' };
        vehicle.slot = slot;
        vehicle.parkingFee = this.calculateFee(1);
        this.data.vehicles.push(vehicle);
        if (vehicle.emergency) {
            this.data.emergencyQueue.push(vehicle);
            this.sortEmergency();
        }
        this.data.history.push({ action: 'add', vehicle, time: new Date().toLocaleString() });
        this.saveData();
        return { success: true, slot, vehicle };
    },
    addVehicle(vehicle) {
        const existing = this.data.vehicles.find(v => v.number === vehicle.number) ||
            this.data.incomingQueue.find(v => v.number === vehicle.number) ||
            this.data.tollQueue.find(v => v.number === vehicle.number) ||
            this.data.emergencyQueue.find(v => v.number === vehicle.number);
        if (existing) return { success: false, message: 'Vehicle already exists in the system.' };

        if (vehicle.emergency) {
            vehicle.tollFee = 0;
            const parked = this.parkVehicle(vehicle);
            if (!parked.success) return parked;
            return { success: true, message: `Emergency vehicle bypassing toll and parked in slot ${parked.slot}` };
        }

        this.enqueueIncoming(vehicle);
        const forwarded = this.forwardIncomingToToll(vehicle.number);
        if (!forwarded) {
            return { success: true, message: 'Vehicle added to incoming queue. Toll queue will process it when space is available.' };
        }

        const tolled = this.dequeueToll();
        if (!tolled) {
            return { success: false, message: 'Failed to process toll.' };
        }

        tolled.tollFee = this.calculateTollFee(tolled.type);
        const parked = this.parkVehicle(tolled);
        if (!parked.success) {
            this.data.incomingQueue = this.data.incomingQueue.filter(v => v.number !== vehicle.number);
            return { success: false, message: 'Parking full after toll. Vehicle remains in incoming queue.' };
        }

        return { success: true, message: `Vehicle ${vehicle.number} paid toll ${tolled.tollFee} and parked in slot ${parked.slot}` };
    },
    removeVehicle(number) {
        const index = this.data.vehicles.findIndex(v => v.number === number);
        if (index === -1) return { success: false, message: 'Vehicle not found.' };
        const vehicle = this.data.vehicles.splice(index, 1)[0];
        const exitToll = this.calculateTollFee(vehicle.type);
        this.data.history.push({ action: 'remove', vehicle, exitToll, time: new Date().toLocaleString() });
        this.data.incomingQueue = this.data.incomingQueue.filter(v => v.number !== number);
        this.data.tollQueue = this.data.tollQueue.filter(v => v.number !== number);
        this.data.emergencyQueue = this.data.emergencyQueue.filter(v => v.number !== number);
        this.saveData();
        return { success: true, message: 'Vehicle removed from slot ' + vehicle.slot + '. Exit toll: ' + exitToll, vehicle };
    },
    searchVehicle(number) {
        return this.data.vehicles.find(v => v.number.toUpperCase() === number.toUpperCase());
    },
    sortEmergency() {
        this.data.emergencyQueue.sort((a, b) => (b.emergency - a.emergency) || a.number.localeCompare(b.number));
    },
    enqueueIncoming(vehicle) {
        this.data.incomingQueue.push(vehicle);
        this.saveData();
    },
    dequeueIncoming() {
        if (!this.data.incomingQueue.length) return null;
        const vehicle = this.data.incomingQueue.shift();
        this.saveData();
        return vehicle;
    },
    enqueueToll(vehicle) {
        if (this.data.tollQueue.length >= 20) return false;
        this.data.tollQueue.push(vehicle);
        this.saveData();
        return true;
    },
    dequeueToll() {
        if (!this.data.tollQueue.length) return null;
        const vehicle = this.data.tollQueue.shift();
        this.saveData();
        return vehicle;
    },
    undoRemoval() {
        for (let i = this.data.history.length - 1; i >= 0; i--) {
            const entry = this.data.history[i];
            if (entry.action === 'remove') {
                this.data.vehicles.push(entry.vehicle);
                if (entry.vehicle.emergency) {
                    this.data.emergencyQueue.push(entry.vehicle);
                    this.sortEmergency();
                }
                this.data.history.splice(i, 1);
                this.saveData();
                return entry.vehicle;
            }
        }
        return null;
    },
    buildBalancedOrder(numbers) {
        if (!numbers.length) return [];
        const mid = Math.floor(numbers.length / 2);
        return [numbers[mid], ...this.buildBalancedOrder(numbers.slice(0, mid)), ...this.buildBalancedOrder(numbers.slice(mid + 1))];
    },
    bindPage() {
        const path = window.location.pathname.toLowerCase();
        const page = path.substring(path.lastIndexOf('/') + 1);

        if (page === 'dashboard.html' || page === 'index.html') {
            this.renderDashboard();
        }
        if (page === 'parking.html') {
            this.bindParking();
            this.renderParking();
        }
        if (page === 'search.html') {
            this.bindSearch();
        }
        if (page === 'traffic.html') {
            this.bindTraffic();
            this.renderTraffic();
        }
        if (page === 'graph.html') {
            this.bindGraph();
            this.renderGraph();
        }
        if (page === 'trees.html') {
            this.bindTrees();
            this.renderTrees();
        }
        if (page === 'history.html') {
            this.bindHistory();
            this.renderHistory();
        }
        this.highlightCurrentNav(page);
    },
    highlightCurrentNav(page) {
        document.querySelectorAll('.nav-list a').forEach(link => {
            if (link.getAttribute('href') === page) {
                link.style.background = 'rgba(59, 130, 246, 0.28)';
            }
        });
    },
    renderDashboard() {
        const active = document.getElementById('activeVehicles');
        const available = document.getElementById('availableSlots');
        const emergency = document.getElementById('emergencyCount');
        const queue = document.getElementById('queueCount');
        const topVehicles = document.getElementById('topVehicles');

        if (active) active.innerText = this.data.vehicles.length;
        if (available) available.innerText = 50 - this.data.vehicles.length;
        if (emergency) emergency.innerText = this.data.emergencyQueue.length;
        if (queue) queue.innerText = this.data.incomingQueue.length + this.data.tollQueue.length;
        if (topVehicles) {
            const numbers = this.data.vehicles.slice(0, 4).map(v => v.number);
            topVehicles.innerHTML = numbers.length ? numbers.join(', ') : 'No active vehicles';
        }
    },
    bindParking() {
        document.getElementById('parkingForm').addEventListener('submit', (event) => {
            event.preventDefault();
            const number = document.getElementById('parkNumber').value.trim();
            const owner = document.getElementById('parkOwner').value.trim();
            const type = document.getElementById('parkType').value;
            const emergency = document.getElementById('parkEmergency').value === 'true';
            const result = this.addVehicle({ number, owner, type, emergency });
            showNotification(result.message);
            if (result.success) {
                this.renderParking();
                event.target.reset();
            }
        });

        document.getElementById('removeForm').addEventListener('submit', (event) => {
            event.preventDefault();
            const number = document.getElementById('removeNumber').value.trim();
            const result = this.removeVehicle(number);
            showNotification(result.message);
            if (result.success) {
                this.renderParking();
                event.target.reset();
            }
        });
    },
    renderParking() {
        const tableBody = document.getElementById('parkingTableBody');
        const countInfo = document.getElementById('parkingStatus');
        if (!tableBody || !countInfo) return;
        tableBody.innerHTML = '';
        const sorted = [...this.data.vehicles].sort((a, b) => a.slot - b.slot);
        sorted.forEach(vehicle => {
            const row = document.createElement('tr');
            row.innerHTML = `<td>${vehicle.slot}</td><td><span class="status-pill occupied">Occupied</span></td><td>${vehicle.number}</td><td>${vehicle.owner}</td><td>${vehicle.type}</td><td>${vehicle.parkingFee ?? 0}</td><td>${vehicle.tollFee ?? 0}</td>`;
            tableBody.appendChild(row);
        });
        countInfo.innerText = `${this.data.vehicles.length} vehicles parked, ${50 - this.data.vehicles.length} slots available`;
    },
    bindSearch() {
        document.getElementById('searchButton').addEventListener('click', () => {
            const number = document.getElementById('vehicleSearch').value.trim();
            const result = this.searchVehicle(number);
            const output = document.getElementById('searchResult');
            if (!output) return;
            if (result) {
                output.innerHTML = `<p><strong>${result.number}</strong> owned by ${result.owner} (${result.type}) in slot ${result.slot}</p>`;
            } else {
                output.innerHTML = '<p>No vehicle found with that number.</p>';
            }
        });
    },
    bindTraffic() {
        document.getElementById('incomingForm').addEventListener('submit', (event) => {
            event.preventDefault();
            const number = document.getElementById('incomingNumber').value.trim();
            const type = document.getElementById('incomingType').value;
            const owner = 'Unknown';
            const emergency = false;
            this.enqueueIncoming({ number, owner, type, emergency });
            showNotification('Incoming vehicle added');
            this.renderTraffic();
            event.target.reset();
        });
        document.getElementById('tollForm').addEventListener('submit', (event) => {
            event.preventDefault();
            const number = document.getElementById('tollNumber').value.trim();
            const type = document.getElementById('tollType').value;
            const owner = 'Unknown';
            const emergency = false;
            const success = this.enqueueToll({ number, owner, type, emergency });
            showNotification(success ? 'Toll vehicle queued' : 'Toll queue is full');
            this.renderTraffic();
            event.target.reset();
        });
        document.getElementById('processIncoming').addEventListener('click', () => {
            const vehicle = this.dequeueIncoming();
            if (vehicle) {
                const forwarded = this.enqueueToll(vehicle);
                showNotification(forwarded ? `Incoming vehicle ${vehicle.number} forwarded to toll` : `Incoming ${vehicle.number} could not be forwarded to toll`);
            } else {
                showNotification('No incoming vehicles');
            }
            this.renderTraffic();
        });
        document.getElementById('processToll').addEventListener('click', () => {
            const vehicle = this.dequeueToll();
            if (vehicle) {
                vehicle.tollFee = this.calculateTollFee(vehicle.type);
                const parked = this.parkVehicle(vehicle);
                if (parked.success) {
                    showNotification(`Toll processed and parked ${vehicle.number} in slot ${parked.slot}`);
                } else {
                    showNotification(`Toll processed but parking failed: ${parked.message}`);
                }
            } else {
                showNotification('No toll vehicles');
            }
            this.renderTraffic();
        });
    },
    renderTraffic() {
        const incomingList = document.getElementById('incomingList');
        const tollList = document.getElementById('tollList');
        const emergencyList = document.getElementById('emergencyList');
        const incomingCount = document.getElementById('incomingCount');
        const tollCount = document.getElementById('tollCount');
        const emergencyCount = document.getElementById('emergencyCount');
        const parkedCount = document.getElementById('parkedCount');

        if (incomingList) {
            incomingList.innerHTML = this.data.incomingQueue.length ? this.data.incomingQueue.map(v => `<li>${v.number} (${v.type})</li>`).join('') : '<li>No incoming vehicles</li>';
        }
        if (tollList) {
            tollList.innerHTML = this.data.tollQueue.length ? this.data.tollQueue.map(v => `<li>${v.number} (${v.type})</li>`).join('') : '<li>No toll vehicles</li>';
        }
        if (emergencyList) {
            emergencyList.innerHTML = this.data.emergencyQueue.length ? this.data.emergencyQueue.map(v => `<li>${v.number} (${v.type})</li>`).join('') : '<li>No emergency vehicles</li>';
        }
        if (incomingCount) incomingCount.innerText = this.data.incomingQueue.length;
        if (tollCount) tollCount.innerText = this.data.tollQueue.length;
        if (emergencyCount) emergencyCount.innerText = this.data.emergencyQueue.length;
        if (parkedCount) parkedCount.innerText = this.data.vehicles.length;
    },
    bindGraph() {
        document.getElementById('runBfs').addEventListener('click', () => {
            const start = Number(document.getElementById('startNode').value);
            const order = this.runBFS(start);
            document.getElementById('graphResult').innerText = order.length ? order.join(' → ') : 'Invalid start node';
        });
        document.getElementById('runDfs').addEventListener('click', () => {
            const start = Number(document.getElementById('startNode').value);
            const order = this.runDFS(start);
            document.getElementById('graphResult').innerText = order.length ? order.join(' → ') : 'Invalid start node';
        });
    },
    renderGraph() {
        const list = document.getElementById('graphList');
        if (!list) return;
        const nodes = this.data.graph.nodes;
        list.innerHTML = '';
        for (let i = 0; i < nodes; i++) {
            const neighbors = this.data.graph.edges.filter(e => e[0] === i || e[1] === i)
                .map(e => (e[0] === i ? e[1] : e[0]));
            list.innerHTML += `<li><strong>Node ${i}:</strong> ${neighbors.join(', ')}</li>`;
        }
    },
    runBFS(start) {
        const nodes = this.data.graph.nodes;
        if (start < 0 || start >= nodes) return [];
        const visited = Array(nodes).fill(false);
        const queue = [start];
        const order = [];
        visited[start] = true;
        while (queue.length) {
            const node = queue.shift();
            order.push(node);
            this.data.graph.edges.forEach(edge => {
                const neighbor = edge[0] === node ? edge[1] : edge[1] === node ? edge[0] : null;
                if (neighbor !== null && !visited[neighbor]) {
                    visited[neighbor] = true;
                    queue.push(neighbor);
                }
            });
        }
        return order;
    },
    runDFS(start) {
        const nodes = this.data.graph.nodes;
        if (start < 0 || start >= nodes) return [];
        const visited = Array(nodes).fill(false);
        const order = [];
        const stack = [start];
        while (stack.length) {
            const node = stack.pop();
            if (visited[node]) continue;
            visited[node] = true;
            order.push(node);
            const neighbors = this.data.graph.edges
                .filter(edge => edge[0] === node || edge[1] === node)
                .map(edge => (edge[0] === node ? edge[1] : edge[0]))
                .sort((a, b) => b - a);
            neighbors.forEach(n => {
                if (!visited[n]) stack.push(n);
            });
        }
        return order;
    },
    bindTrees() {
        document.getElementById('treeRefresh').addEventListener('click', () => this.renderTrees());
    },
    renderTrees() {
        const numbers = this.data.vehicles.map(v => v.number).sort();
        const bstOut = document.getElementById('bstList');
        const avlOut = document.getElementById('avlList');
        const balanced = this.buildBalancedOrder(numbers);
        if (bstOut) bstOut.innerHTML = numbers.length ? numbers.join(', ') : 'No vehicles in BST';
        if (avlOut) avlOut.innerHTML = balanced.length ? balanced.join(', ') : 'No vehicles in AVL';
    },
    bindHistory() {
        document.getElementById('undoButton').addEventListener('click', () => {
            const restored = this.undoRemoval();
            showNotification(restored ? `Restored ${restored.number}` : 'No removal to undo');
            this.renderHistory();
            this.renderParking();
        });
    },
    renderHistory() {
        const historyList = document.getElementById('historyList');
        if (!historyList) return;
        historyList.innerHTML = this.data.history.length ? this.data.history.map(item => {
            const tollInfo = item.exitToll ? ` | Exit toll: ${item.exitToll}` : item.vehicle.tollFee ? ` | Toll paid: ${item.vehicle.tollFee}` : '';
            return `<li>${item.time}: ${item.action.toUpperCase()} ${item.vehicle.number} (${item.vehicle.owner})${tollInfo}</li>`;
        }).join('') : '<li>No history yet</li>';
    }
};

document.addEventListener('DOMContentLoaded', () => SmartCityUI.init());

function showNotification(message) {
    const notification = document.createElement('div');
    notification.className = 'toast';
    notification.innerText = message;
    document.body.appendChild(notification);
    requestAnimationFrame(() => notification.classList.add('visible'));
    setTimeout(() => {
        notification.classList.remove('visible');
        setTimeout(() => notification.remove(), 300);
    }, 2200);
}