Checklist to complete before we can release an open source project.

The aim of this checklist is to be the lightest-weight set of things we can check in order to expediently Open Source projects. It doesn’t replace the decision about what should be released, but rather applies after that decision has been made. In order to ensure we can be as open as possible, we’d like to keep this list as small and pragmatic as is legally viable.

Licensing
[x] It has a valid open source license (our preferred OSS license is MIT, we accept Apache-2 as well. Other licences require discussion)
[ ] None of the dependencies have a software license that affects the top-level project license
    [ ] TODO: Should we list here some tools that could be used to traverse the dependency licenses. Do we need to do that?
    [ ] TODO: What does this really mean? Can an MIT project depend on a GPL one, or does it need to be MIT all-the-way down? Is there a difference between releasing the library on GH and releasing a tool built with it?
CI
[x] CI scripts do not contain any passwords or secrets
[x] CI SaaS project does not contain any env var that could leak
    [x] If there are env vars, the CI project is configured to not trust forks and PRs
[x] CI SaaS global settings do not contain any env var or secret shared to this project that could leak
GitHub
[x] The project has a descriptive README.md file rendered on the GitHub repo entry page
[x] The GH description (underneath the repo title) is clear and concise
[x] (Optional) If appropriate, there is a link to a relevant website next to the GH description
[x] (Optional) Add topics to the repository 
[ ] GitHub Releases are used to mark releases
[x] GitHub correctly interprets and lists the Licence file, if using a GitHub supported licence
Project
[x] There is a LICENSE file including SPDX licence identifier https://spdx.org/licenses/
[x] There is a appropriate documentation
[ ] There are contributing guidelines (preferably in a CONTRIBUTING file) including recognition that contributions are under a contributor licence agreement
[ ] Add a code of conduct or link to one approved by the foundation
[x] There is a .gitignore file excluding everything we don’t want versioned
[ ] The project is versioned (the preferred versioning system is Semantic Versioning 2.0.0)
[ ] Releases are git tagged with a description
[ ] There is some changelog captured, either in a CHANGELOG file at the root of the repository, or as GH releases descriptions


Content

Check commits, documentation, code and go through the git history to check for the following:

[x] There is no swearing or anything offensive (use personal judgement and remember that young people and Foundation partners can read this)
[x] There is no confidential information
[x] There are no password, secrets or similar (pay special attention to the git history in case something was accidentally added and removed)
[x] None of our partner or founder partner names are mentioned in any context except entirely factual (“This supports Company XYZ’s new toy”, “This includes library developed by XYZ” are acceptable, but “Because XYZs library is broken” is not). Where discussions of partners in other than positive or neutral light are necessary, sign-off should be sought from the management team to ensure appropriate relationship management steps can be taken. This is a judgement call to be left to the team publishing the code, who we trust to flag when necessary. 
Headers of Files
[ ] Each file should contain at least the following information as described in the SPDX documentation https://github.com/david-a-wheeler/spdx-tutorial/blob/master/README.md#spdx-license-expressions-in-source-code-files
    (c) [year file created] - [last year file modified], Micro:bit Educational Foundation and the [project name] contributors
    SPDX-License-Identifier: [SPDX license expression]
    
Assets
1. When releasing projects, it is best practice to avoid including assets that are visually ‘The Foundation’ - for example we split ‘dev-docs’ and ‘tech_microbit_org’. However, where there are images that must be included, these should be under a different licence to the code if they have different copyright
2. Specifically, images of the micro:bit should be CC-4.0-BY-NC-SA or just (C) Micro:bit Educational Foundation
3. TODO: How do we appropriately protect logos etc that are part of the documentation? Do we need to split our projects? I think we need to ask a Lawyer 

When docs are licenced differently to the rest of the project (eg MIT for project, CC for docs
