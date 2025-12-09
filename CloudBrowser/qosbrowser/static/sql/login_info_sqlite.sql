CREATE TABLE IF NOT EXISTS login_info (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  name TEXT NOT NULL,
  secret_id TEXT NOT NULL,
  secret_key TEXT NOT NULL,
  remark TEXT,
  timestamp INTEGER NOT NULL,
  UNIQUE(name, secret_id)
);

