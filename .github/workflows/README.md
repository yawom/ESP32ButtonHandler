# GitHub Actions Workflows

This directory contains automated workflows for the ESP32ButtonHandler library.

## Workflows

### 📦 platformio-publish.yml

Automatically publishes the library to the PlatformIO registry when a version tag is pushed.

**Triggers:**
- Version tags (e.g., `v2.0.0`, `v2.1.0`)
- Manual trigger via GitHub Actions UI

**What it does:**
1. Validates `library.json` format and structure
2. Checks library directory structure
3. Verifies version consistency between tag and `library.json`
4. Publishes to PlatformIO registry
5. Creates a GitHub release with installation instructions

**Requirements:**
- GitHub Secret: `PLATFORMIO_AUTH_TOKEN` (get from `pio account token`)

**Usage:**
```bash
# Update versions in library.json and library.properties first
git tag v2.0.1
git push origin v2.0.1
```

### ✅ ci.yml

Validates the library on every push and pull request.

**Triggers:**
- Push to `main` or `master` branch
- Pull requests targeting `main` or `master`

**What it does:**
1. Validates `library.json` and `library.properties`
2. Checks library structure (directories, required files)
3. Validates example sketches exist and are properly structured
4. Checks code quality (tabs, whitespace, line endings)

**No setup required** - runs automatically on all PRs and pushes.

## Setting Up PlatformIO Publishing

1. **Get your PlatformIO token:**
   ```bash
   pio account token
   ```

2. **Add to GitHub Secrets:**
   - Navigate to: Repository → Settings → Secrets and variables → Actions
   - Click "New repository secret"
   - Name: `PLATFORMIO_AUTH_TOKEN`
   - Value: Your token from step 1

3. **Publish a release:**
   ```bash
   # Update version in library.json and library.properties
   vim library.json
   vim library.properties

   # Commit changes
   git add library.json library.properties
   git commit -m "Bump version to 2.0.1"

   # Create and push tag
   git tag v2.0.1
   git push origin main
   git push origin v2.0.1
   ```

The workflow will automatically publish to PlatformIO and create a GitHub release!

## Troubleshooting

**"PLATFORMIO_AUTH_TOKEN not set" error:**
- Make sure you added the secret correctly in GitHub Settings
- Check the secret name matches exactly: `PLATFORMIO_AUTH_TOKEN`

**Version mismatch error:**
- Ensure `library.json` version matches your git tag (without the 'v' prefix)
- Tag: `v2.0.1` → library.json: `"version": "2.0.1"`

**Publishing fails:**
- Check that you have permissions on the PlatformIO account
- Verify your token hasn't expired: `pio account token`
- Check the GitHub Actions logs for detailed error messages
